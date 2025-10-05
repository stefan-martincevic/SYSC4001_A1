/**
 *
 * @file interrupts.cpp
 * @author Stefan Msrtincevic Sadman Sajid
 *
 */

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int current_time = 0;
    execution = "Time of the event,Duration of the event,Event type\n";


    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU") // process CPU 
        {
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", " + activity + "\n";
            current_time += duration_intr;
        }

        else // process interrupts
        {
            std::pair<std::string, int> result = intr_boilerplate(current_time, duration_intr, 10, vectors);
            execution += result.first;
            current_time = result.second;
        }


        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
