#pragma once

namespace input
{
    /*
     * returns true when input is ready to be read
     * returns false on timeout or input not ready to be read
     */
    bool process_input(uint32_t timeout_ms);

    /*
     * copy at most bufsize chars of the string to read into the buf
     */
    void read(char* output_buffer, uint16_t output_buffer_cch);
}
