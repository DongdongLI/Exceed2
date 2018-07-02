#include <iostream>
#include <thread>
using namespace std;

void foo(int num){
	for (int i = 0; i < num; i++){
		cout << "Thread using function pointer as callable. \n";
	}
}

/*class thread_obj {
public:
	void operator()(int num){
		for (int i = 0; i < num; i++)
			cout << "Thread using function object as callable. \n";
	}
};
/*
int main(){
	cout << "3 threads running independently." << endl;

	thread thread_1(foo, 3);
	thread thread_2(thread_obj(), 3);

	auto f = [](int num){
		for (int i = 0; i < num; i++)
			cout << "Thread using lambda expression as callable\n";
	};

	thread thread_3(f, 3);

	thread_1.join();
	thread_2.join();
	thread_3.join();
	system("PAUSE");
	return 0;
}
*/