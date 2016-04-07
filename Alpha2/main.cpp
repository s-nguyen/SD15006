#include "leptonSPI.h"
#include "uart_socket.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    leptonSPI c;
    CUARTSocket a("/dev/ttyGS0", 115200);
    while (c.getFrame() != 59) {}
    for(int i = 0; i < 10; i++){
        cout << c.frame[i] << endl;
    }
    a.Open();

    cout << a.Write(c.frame, 9600);

    getchar();


    return 0;
}
