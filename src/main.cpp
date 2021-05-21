#include <cstddef>
#include <owltypes.h>
#include <libowl.h>

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std::literals;

using JVM = graal_isolatethread_t *;
using Isolate = graal_isolate_t *;
using OwlFormula = void *;

namespace cli {
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
}

[[noreturn]]
void fatal(const char *message) {
    std::cerr << message << "\n";
    exit(1);
}

std::vector<std::string> parse_vars(int &argc, char **&argv) 
{
    if(argc < 1)
        fatal("Please specify at least one variable");

    std::vector<std::string> vars;

    while(argc > 0 && argv[0][0] != '-') {
        vars.push_back(argv[0]);
        argc--;
        argv++;
    }

    return vars;
}

void parse_command_line(int argc, char **argv) 
{
    argc--; // discard command name
    argv++;

    if(argc < 1)
        fatal("Too few arguments");

    if(argv[0] == "-i"s) {
        argc--;
        argv++;
        cli::inputs = parse_vars(argc, argv);
    }

    if(argv[0] == "-o"s) {
        argc--;
        argv++;
        cli::outputs = parse_vars(argc, argv);
    }
}

OwlFormula parse_formula(JVM owl, std::string const& str) {
    int nvars = cli::inputs.size() + cli::outputs.size();
    auto vars = std::make_unique<char *[]>(nvars);

    for(int i = 0; i < cli::inputs.size(); ++i)
        vars[i] = const_cast<char *>(cli::inputs[i].c_str());

    for(int i = 0; i < cli::outputs.size(); ++i)
        vars[i + cli::inputs.size()] = 
            const_cast<char *>(cli::outputs[i].c_str());        
    
    return 
    ltl_formula_parse(owl, const_cast<char*>(str.c_str()), vars.get(), nvars);
}

void print_formula(std::ostream &out, JVM owl, OwlFormula formula) {
    
    size_t size = 2;
    size_t len = 0;
    std::unique_ptr<char[]> str;
    
    do
    {
        size = size * 2;
        str = std::make_unique<char[]>(size);
        len = print_object_handle(owl, formula, str.get(), size);
    } while (len >= size);
    
    out << str.get() << "\n";
}

OwlFormula simplify_formula(JVM owl, OwlFormula formula) {
    size_t nvars = cli::inputs.size() + cli::outputs.size();
    auto statuses = std::make_unique<int[]>(nvars);

    return ltl_formula_simplify(
        owl, formula, cli::inputs.size(), statuses.get(), nvars
    );
}

int main(int argc, char **argv) 
{
    parse_command_line(argc, argv);

    Isolate isolate = nullptr;
    JVM owl = nullptr;

    if(graal_create_isolate(nullptr, &isolate, &owl) != 0)
        fatal("Unable to launch GraalVM");

    assert(isolate != nullptr);
    assert(owl != nullptr);

    std::string line;
    std::getline(std::cin, line);

    OwlFormula formula = parse_formula(owl, line);

    formula = simplify_formula(owl, formula);

    print_formula(std::cout, owl, formula);

    graal_tear_down_isolate(owl);

    return 0;
}