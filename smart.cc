#include <memory>
#include <iostream>
using namespace std;

class LL {
    int data;
    shared_ptr<LL> next;
    
    public:
    LL(int new_data, shared_ptr<LL> new_next) : data(new_data), next(new_next) {
	cout << "Making an LL with value: " << data << endl;
    }
    ~LL() {
	cout << "~LL with value: " << data << endl;
    }
    void set_data(int new_data) {
	data = new_data;
    }
    int get_data() const {
	return data;
    }
};

shared_ptr<LL> make_a_list() {
    //shared_ptr<LL> one(new LL(1,nullptr));
    auto one = make_shared<LL>(1,nullptr);
    cout << "Changing one to 42.\n";
    one->set_data(42);
    auto two = make_shared<LL>(2,one);
    auto three = make_shared<LL>(3,two);
    auto four = make_shared<LL>(4,three);
    //shared_ptr<LL> two(new LL(2,one));
    //shared_ptr<LL> three(new LL(3,two));
    //shared_ptr<LL> four(new LL(4,three));
    return four;
}

int main() {
    cout << "Hello, my name is Inigo Montoya.\n";
    auto root = make_a_list();
    //make_a_list();
    cout << "You kill my father. Prepare to die.\n";
} //Deconstructor called here
