#pragma once

#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ----------------------------------------------------------------------------
// Handle command line options
//
// Options:
//  [-help]                 Show help menu
//  [-cpucmp]               CPU_COMPARE flag
//  [-check]                CHECK flag
//  [-profiling]            PROFILING flag
//  [-devinfo]              DEVICE_INFO flag               
// ----------------------------------------------------------------------------

class Command_line_options final {
private:   
    po::options_description options_;
    bool device_info_flag_;
    bool cpu_compare_flag_;
    bool check_flag_;
    bool profiling_flag_;

    void setup();
public:
    enum class Return_status {  
        OPTS_SUCCESS,
        OPTS_HELP,
        OPTS_FAILURE
    };

    Command_line_options();
    
    Command_line_options (const Command_line_options &rhs) = delete;
    Command_line_options &operator= (const Command_line_options &rhs) = delete;
    Command_line_options (Command_line_options &&rhs) = delete;
    Command_line_options &operator= (Command_line_options &&rhs) = delete;
    ~Command_line_options() = default;
        
    Return_status parse(int argc, char* argv[]);

    bool device_info_flag() { return device_info_flag_; }
    bool cpu_compare_flag() { return cpu_compare_flag_; }
    bool check_flag()       { return check_flag_; }
    bool profiling_flag()   { return profiling_flag_; }
};

Command_line_options::Command_line_options() : options_(""),
                                               device_info_flag_(false),
                                               cpu_compare_flag_(false), 
                                               check_flag_(false), 
                                               profiling_flag_(false) 
{
   setup();
}

void Command_line_options::setup()
{
   po::options_description options( "Program Options" );
   options.add_options()
         ("help", "Display help menu")
         ("devinfo", po::bool_switch(&device_info_flag_)->default_value(false), "DEVICE_INFO-flag")
         ("cpucmp", po::bool_switch(&cpu_compare_flag_)->default_value(false), "CPU_COMPARE-flag")
         ("check", po::bool_switch(&check_flag_)->default_value(false), "CHECK-flag")
         ("profiling", po::bool_switch(&profiling_flag_)->default_value(false), "PROFILING-flag");

   options_.add(options);
}

Command_line_options::Return_status Command_line_options::parse(int argc, char* argv[])
{
    Return_status ret = Return_status::OPTS_SUCCESS;

    po::variables_map varMap;

    try {
        po::store(po::parse_command_line(argc, argv, options_), varMap);
        po::notify(varMap);

        // Help option
        if (varMap.count("help")) {
            std::cout << options_ << std::endl;
            return Return_status::OPTS_HELP;
        }
    }
    catch (std::exception &e) {
        std::cout << "ERROR - parsing error: " << e.what() << std::endl;
        ret = Return_status::OPTS_FAILURE;
    }

    return ret;
}

