#include "leptonSPI.h"
#include "uart_socket.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    leptonSPI c;
    system("modprobe g_serial");
    CUARTSocket a("/dev/ttyGS0", 115200);
    while (c.getFrame() != 59) {}

    a.Open();

    a.Write(c.frame, 9840);



   a.Wait();

    a.Close();

    return 0;
}
