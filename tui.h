#pragma once

#include <vector>
#include <thread>

namespace tui
{
    /*
     * initialize the tui module
     * Returns the thread associated used for drawing.
     */
    std::thread init();

    /*
     * Write a character to the input field
     * c: the character to write
     */
    void push_to_input_field(char c);

    /*
     * Write a character to the input field
     * c: the character to write
     */
    void pop_from_input_field();

    /*
     * Write a msg to the conversation thread
     * msg: the null delimited string to write
     */
    void write_msg_to_conversation_thread(const char* msg);

    /*
     * Clear the input buffer
     */
    void clear_input();
}
