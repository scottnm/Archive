#pragma once

namespace tui
{
    /*
     * initialize the tui module
     */
    void init();

    /*
     * Write a character to the input field
     * c: the character to write
     */
    void write_to_input_field(char c);

    /*
     * Clear the input field
     */
    void clear_input_field(void);

    /*
     * Write a msg to the conversation thread
     * msg: the null delimited string to write
     */
    void write_msg_to_conversation_thread(const char* msg);
}
