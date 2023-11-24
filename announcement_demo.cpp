// announcement_demo.cpp

#include "Announcement.h"
#include <iostream>

using namespace std;

int main()
{
    Announcement a1("yumyum", Rank::SANTA, "I love Christmas!");
    cout << a1 << endl; // prints: {yumyum, santa, "I love Christmas!"}

    // copy another Announcement
    Announcement a2(a1);
    cout << a2 << endl; // prints: {yumyum, santa, "I love Christmas!"}

    // parse a string of the form "sender_name rank text"
    Announcement a3("yumyum santa I love Christmas!");
    cout << a3 << endl; // prints: {yumyum, santa, "I love Christmas!"}
}


//  void printQ(Queue & temp){
//         if(temp.empty()){
//             cout << "empty" << endl;

//         }else{
//            int i = 0;
//            while(i < temp.size()){
//             cout << temp.front().get_sender_name() << ": " <<temp.front().get_text() << " " << endl;
//             Announcement num = temp.front();
//             temp.dequeue();
//             temp.enqueue(num);
//             i++;
//            }
//            cout << endl;

//         }
//     }
//     void printQQ(){
//         cout << "santa: " << endl;
//         printQ(santa);
//         cout << "reindeer: " << endl;
//         printQ(reindeer);
//         cout << "elf2: " << endl;
//         printQ(elf2);
//         cout << "elf1: " << endl;
//         printQ(elf1);
//         cout << "snowman: " << endl;
//         printQ(snowman);

//     }