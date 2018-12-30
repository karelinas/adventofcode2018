#include "cpu.hpp"
#include "program.hpp"

#include <boost/program_options.hpp>

#include <cstddef>
#include <fstream>
#include <optional>
#include <iostream>

int main(int argc, char* argv[])
{
    namespace po = boost::program_options;
    using aoc::cpu::Cpu;
    using aoc::cpu::Program;
    using aoc::cpu::ExitStatus;

    po::options_description desc("Options");
    desc.add_options()
        ("help", "show help message")
        ("gpr0,r", po::value<int>(), "initialize gpr0 with a value")
        ("state,s", po::value<std::string>(), "use state file")
        ("program,p", po::value<std::string>(), "load a program");
    po::positional_options_description positional_desc;
    positional_desc.add("program", 1);
    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv)
                    .options(desc)
                    .positional(positional_desc)
                    .run(), vm);
        po::notify(vm);
    } catch (po::error e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    if (argc == 1 || vm.count("help")) {
        std::cout << "Usage: " << argv[0] << " [OPTIONS] [PROGRAM]\n";
        std::cout << desc << "\n\n";
        std::cout << "  Must specify either a program or an existing statefile\n";
        return EXIT_FAILURE;
    }

    std::optional<std::string> statefilename;
    if (vm.count("state")) {
        statefilename = vm["state"].as<std::string>();
    }

    std::optional<Program> program{};
    std::optional<Cpu> cpu{};
    if (vm.count("program")) {
        auto filename = vm["program"].as<std::string>();
        std::ifstream programfile(filename);
        if (!programfile) {
            std::cerr << "Failed to open file: " << filename << '\n';
            return EXIT_FAILURE;
        }
        program = Program::from_stream(programfile);
        cpu = Cpu();
        std::cout << "Loaded program from file\n";
    } else if (statefilename) {
        std::ifstream statefile(*statefilename, std::ifstream::binary);
        if (statefile) {
            cpu = Cpu::unserialize(statefile);
            program = Program::unserialize(statefile);
            std::cout << "Loaded state from file\n";
        }
    }

    if (!program) {
        std::cerr << "No program found. Must specify either a program file or an existing state file\n";
        return EXIT_FAILURE;
    }

    if (program->errors.size() > 0) {
        std::cerr << "Program has errors\n";
        for (const auto& error : program->errors) {
            std::cerr << "Error on line " << error.linenumber << ": " << error.what << '\n';
        }
        return EXIT_FAILURE;
    }

    if (vm.count("gpr0")) {
        auto gpr0 = vm["gpr0"].as<int>();
        cpu->gpr[0] = gpr0;
        std::cout << "Set gpr0 = " << gpr0 << '\n';
    }

    std::cout << "Running program\n";

    auto exitstatus = cpu->run_program(*program, &(std::cout));
    switch (exitstatus) {
    case ExitStatus::Ok:
        std::cout << "Program finished\n";
        std::cout << "Register 0 value: " << cpu->gpr[0] << '\n';
        break;
    case ExitStatus::Interrupted: {
        std::cout << "Program interrupted after " << cpu->cycle_count << " instructions\n";
        if (statefilename) {
            std::ofstream outfile(*statefilename, std::ofstream::binary);
            if (!outfile) {
                std::cerr << "Error: Failed to open statefile: " << *   statefilename << '\n';
                return EXIT_FAILURE;
            }
            cpu->serialize(outfile);
            program->serialize(outfile);
            std::cout << "State saved to " << *statefilename << "\n";
        }
        break;
    }
    case ExitStatus::Error:
        std::cerr << "Program exited with error\n";
        return EXIT_FAILURE;
    };
    return EXIT_SUCCESS;
}
