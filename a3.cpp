// a3.cpp

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : Yanjun Qian
// St.# : 301551196
// Email: yanjun_qian@sfu.ca
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

//
// Do not #include any other files!
//
#include "Announcement.h"
#include "JingleNet_announcer.h"
#include "Queue_base.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;


// template <typename T>
class Queue: public Queue_base<Announcement>{
    
    private:
    
    //set the node be private 
    struct Node{

        Announcement data;
        Node* next;
        
    };

    Node* head;
    Node* tail;
    int count;

    public:
    
    //default constructor 
    Queue(){

        head = nullptr;
        tail = nullptr;
        count = 0;
        
    }

    //return the size of the linked list 
    int size()const{

        return count;
    }

    //insert new node to the tail 
    void enqueue(const Announcement &item){

        //create new node
        Node* newnode = new Node{item,nullptr};

        //set the tail's next 
        if(tail != nullptr){
            tail->next = newnode;
        }
        //set the newnode as the tail 
        tail= newnode;
        
        //if the list is empty before insert
        //set the head
        if(head == nullptr){
            head = newnode;
        }
        count++;
           
    }
    
    //return the front node's data 
    const Announcement &front() const{
       
        if(head == nullptr){

            throw runtime_error("dequeue: queue is empty");

        }else{
            //rreturn the front's data 
            return head->data;
        }

    }

    //remove the front node 
    void dequeue(){

        if(empty()){
            throw runtime_error("dequeue: queue is empty");
        }else{

            Node* remove = head;
            //reset the head
            head = head->next;
            
            //if only one node in the list
            if(head == nullptr){
                //set the tail as nullptr
                tail = nullptr;
            }

            delete remove;
            count--;
        }

    }
    
    //helper function for checking size 
    bool empty(){

        return count == 0;
    }


    //destructor
    ~Queue(){
        //if it is not empty, delete all the node 
        if(!empty()){
            Node* temp = head;
            while(temp != nullptr){
                Node* remove = temp;
                temp = temp->next;
                delete remove;
            }
        }

    }

};

class JingleNet{
    
    private:
    
    //set these five queue as private
    Queue santa;
    Queue reindeer;
    Queue elf1;
    Queue elf2;
    Queue snowman;

    public:
    
    //consructor
    JingleNet(){};

    //destructor 
    ~JingleNet(){};

    //function that work for corresponding command 
    void achivecommand(string& command,string& line){
        //get the sender name from the line that read from file
        size_t pos = line.find(' ');
        string name = line.substr(0, pos);

        if(command == "SEND"){
           //insert the infor into corresponding queue
           send(line);

        }else if(command == "REMOVE_ALL"){
           //remove all the infor of this sender
           remove_all(name);

        }else if(command == "PROMOTE_ANNOUNCEMENTS"){
            //prome all the infor of this sender
            prome_announcents(name);

        }else if(command == "ANNOUNCE"){

            //get the number of announce    
            int num = stoi(line);
            announce(num);

        }
   
    }

    //work for command of send 
    void send(string line){
        
        //create the data that store the announce
        Announcement newAnnoun(line);
        //add it into corresponding queue 
        add_queue(newAnnoun.get_rank(), newAnnoun);
      
    }

   //helper function for adding the infor into corresponding queue 
   //depend on its corresponding rank
   void add_queue(Rank r, const Announcement &item)
    {
        switch (r)
        {
        case Rank::SNOWMAN:
            snowman.enqueue(item);
            break;
        case Rank::ELF1:
            elf1.enqueue(item);
            break;
        case Rank::ELF2:
            elf2.enqueue(item);
            break;
        case Rank::REINDEER:
            reindeer.enqueue(item);
            break;
        case Rank::SANTA:
            santa.enqueue(item);
            break;
        default:
            assert(false);
        }
    }

    //helper function that get the total number of node from all queue 
    int size_total(){
        return (santa.size() + reindeer.size() + elf1.size() + elf2.size()
               + snowman.size());
    }

    //helper function for announce 
    int announce_inner(Queue & q, int size){
        int temp = size;
        //compare the size to get the less size for dequeue 
        if(q.size()<size){
            temp = q.size();
        }
        
        for(int i = 0; i < temp; i++){
            //get the data
            Announcement add = q.front();
            q.dequeue();
            //announce it into text
            jnet.announce(add);
        }
        //return the number announce still need
        //to announce in the follow queue 
        return (size - temp);
    }

  

    void announce(int num){
        
        //compare the size and get the smaller size
        if(size_total() < num){
            num = size_total();
        }
        //announce the infor 
        int j = announce_inner(santa, num);
        j = announce_inner(reindeer, j);
        j = announce_inner(elf2, j);
        j = announce_inner(elf1, j);
        j = announce_inner(snowman, j);
  
    }
    
    //helper function for the remove_all command 
    void remove(Queue& t1, string name){
           //if the queue is empty, do nothing 
           if(!t1.empty()){
            int j = 0;
            int i = t1.size();
            //dequeue the data to check 
            while(j < i){
                //get the data 
                Announcement t2 = t1.front();
                t1.dequeue();
                //if not the remove person, then add it back
                if(t2.get_sender_name()!=name){
                    t1.enqueue(t2);
                }
                j++;

            }

        } 
    }
    
    //work for remove_all command 
    void remove_all(string name){
         
        remove(santa,name);
        remove(reindeer,name);
        remove(elf2,name);
        remove(elf1,name);
        remove(snowman,name);

    }
    //helper function for prome_announcents(
    void promote(Queue& t1, Queue& t2, string name, string rank){
        //if the queue is empty do nothing 
        if(!t2.empty()){

            int j = 0;
            int i = t2.size();

            while(j < i){
                //get the data 
                Announcement data = t2.front();
                t2.dequeue();
                if(data.get_sender_name()!=name){
                    //if not the promote person, enqueue back to the orginal queue
                    t2.enqueue(data);
                }else{
                    //if it is the promote person, change the rank 
                    Announcement inputdata(data.get_sender_name(),
                                   to_rank(rank),data.get_text());
                    //put it into a higer level queue 
                    t1.enqueue(inputdata);
                }
                j++;    
            }

        }
    }
    
    //work for the prome_announcents command 
    void prome_announcents(string name){
        
        promote(santa,reindeer,name,"santa");
        promote(reindeer,elf2,name,"reindeer");
        promote(elf2,elf1,name,"elf2");
        promote(elf1,snowman,name,"elf1");

    }

};


int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1; 
    }

    string filename = argv[1];
    ifstream infile(filename);
    string line;
    int num_lines = 0;
    
    //initial the Jinlenet 
    JingleNet collectedqueue;

    while (getline(infile, line))
    {
        num_lines++;

        //get the command from the reading line 
        size_t pos = line.find(' ');
        string command = line.substr(0,pos);
        
        //get the line that except the command 
        line = line.substr(pos + 1);  
        
        //put it into achivecommand that work for corresponding command 
        collectedqueue.achivecommand(command, line);
        
    }


    return 0;


}


