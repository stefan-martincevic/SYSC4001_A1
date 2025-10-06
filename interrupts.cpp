/**
 *
 * @file interrupts.cpp
 * @author Stefan Martincevic, Sadman Sajid
 *
 */

#include <interrupts.hpp>

int main(int argc, char **argv)
{

    // vectors is a C++ std::vector of strings that contain the address of the ISR
    // delays  is a C++ std::vector of ints that contain the delays of each device
    // the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;     //!< string to store single line of trace file
    std::string execution; //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/

    int current_time = 0;
    int context_switch_time = 10; // number has to be changed to 20, 30 and rerun simulation

    execution = "Time of the event,Duration of the event,Event type\n";

    /******************************************************************/

    // parse each line of the input trace file
    while (std::getline(input_file, trace))
    {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU") // process CPU
        {
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", CPU burst\n";
            current_time += duration_intr;
        }

        else if (activity == "SYSCALL") // process system call
        {
            std::pair<std::string, int> result = intr_boilerplate(current_time, duration_intr, context_switch_time, vectors);
            execution += result.first;
            current_time = result.second;

            int intr_duration = delays[duration_intr];
            execution += std::to_string(current_time) + ", " + std::to_string(intr_duration) + ", call device driver\n";
            current_time += intr_duration;

            execution += std::to_string(current_time) + ", " + std::to_string(context_switch_time) + ", Context switch\n";
            current_time += context_switch_time;

            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", Execute IRET\n";
            current_time += 1;
        }
        else if (activity == "END_IO") // process IO
        {
            std::pair<std::string, int> result = intr_boilerplate(current_time, duration_intr, context_switch_time, vectors);
            execution += result.first;
            current_time = result.second;

            int intr_duration = delays[duration_intr];
            execution += std::to_string(current_time) + ", " + std::to_string(intr_duration) + ", Store information in memory\n";
            current_time += intr_duration;

            execution += std::to_string(current_time) + ", " + std::to_string(context_switch_time) + ", Context switch\n";
            current_time += context_switch_time;

            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", Execute IRET\n";
            current_time += 1;
        }

        /************************************************************************/
    }

    input_file.close();

    write_output(execution);

    return 0;
}
