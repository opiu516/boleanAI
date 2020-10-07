#include<iostream>
#include<cmath>
#include<fstream>
#include <stdlib.h> 
#include<string.h>
using namespace std;

class brain{
	private:
		int number_input;
		int number_hidden;
		int number_output;
		float **weights_hidden;
		float **weights_output;
		float *biases_hidden;
		float *biases_output;
		char *file_name;
		float rate = 0.1;
	public:
		brain(){
			get_info();
			cout<<"allocating memory..."<<endl;
			get_file_name();
 			weights_hidden = new float*[number_input];
			for(int i = 0;i < number_input;i++) 
				weights_hidden[i] = new float[number_hidden];
				
			weights_output = new float*[number_hidden];
			for(int i = 0;i < number_hidden;i++)
				weights_output[i] = new float[number_output];
				
			biases_hidden = new float[number_hidden];
			biases_output = new float[number_output];
			cout<<"allocation successful!"<<endl;
			cout<<"searching for file..."<<endl;
			if(test_file())
				{
				cout<<"file found. Getting weights"<<endl;
				get_weights();
				}
			else
				{
				cout<<"file not found. Randomizing weights"<<endl;	
				get_new_weights();
				}
		}
		~brain(){	
			cout<<"saving weights..."<<endl;
			save_weights();
			cout<<"save successful!"<<endl;
		
			cout<<"clearing memory..."<<endl;
			for(int i = 0;i < number_input;i++) 
				delete[] weights_hidden[i];
			delete[] weights_hidden;
			
			for(int i = 0;i < number_hidden;i++)
				delete[] weights_output[i];
			delete[] weights_output;
			
			delete[] biases_hidden;
			delete[] biases_output;	
			delete[] file_name;
			cout<<"memory clear!"<<endl;
		}
		void get_info(){
			cout<<"number of inputs - ";
			cin>>number_input;
			cout<<"number of hidden - ";
			cin>>number_hidden;
			cout<<"number of outputs - ";
			cin>>number_output;
		}
		void get_file_name(){
			int name_lenght = 13;
			name_lenght += number_lenght(number_input);
			name_lenght += number_lenght(number_hidden);
			name_lenght += number_lenght(number_output);
			
			file_name = new char[name_lenght];
			
			ofstream temp_out;
			temp_out.open("temp.txt");
			temp_out<<"weights_"<<number_input<<number_hidden<<number_output<<".txt";
			temp_out.close();
			
			ifstream temp_in;
			temp_in.open("temp.txt");
			temp_in>>file_name;
			temp_in.close();						
		}
		int number_lenght(int number){
			int lenght = 0;
			while(number > 0){
				number = number / 10;
				lenght++;
			}
			return lenght;
		}
		void get_new_weights(){
			for(int i = 0;i < number_input;i++)
				for(int j = 0;j < number_hidden;j++)
					weights_hidden[i][j] = (1 + rand()%100) / 100.0;
					
			for(int i = 0;i < number_hidden;i++)
				for(int j = 0;j < number_output;j++)
					weights_output[i][j] = (1 + rand()%100) / 100.0;
			
			for(int i = 0;i < number_hidden;i++)biases_hidden[i] = (1 + rand()%100) / 100.0;
			for(int i = 0;i < number_output;i++)biases_output[i] = (1 + rand()%100) / 100.0;
		}
		void get_weights(){
			ifstream doc;
			doc.open(file_name);
			
			for(int i = 0;i < number_input;i++)
				for(int j = 0;j < number_hidden;j++)
					doc>>weights_hidden[i][j];
					
			for(int i = 0;i < number_hidden;i++)
				for(int j = 0;j < number_output;j++)
					doc>>weights_output[i][j];
			
			for(int i = 0;i < number_hidden;i++)doc>>biases_hidden[i];
			for(int i = 0;i < number_output;i++)doc>>biases_output[i];
			
			doc.close();
		}
		void save_weights(){
			ofstream doc;
			doc.open(file_name);
			
			for(int i = 0;i < number_input;i++)
				for(int j = 0;j < number_hidden;j++)
					doc<<weights_hidden[i][j]<<" ";
					
			for(int i = 0;i < number_hidden;i++)
				for(int j = 0;j < number_output;j++)
					doc<<weights_output[i][j]<<" ";
			
			for(int i = 0;i < number_hidden;i++)doc<<biases_hidden[i]<<" ";
			for(int i = 0;i < number_output;i++)doc<<biases_output[i]<<" ";
			
			doc.close();
		}
		void display_weights(){
			for(int i = 0;i < number_input;i++)
				for(int j = 0;j < number_hidden;j++)
					cout<<weights_hidden[i][j]<<" ";
					
			for(int i = 0;i < number_hidden;i++)
				for(int j = 0;j < number_output;j++)
					cout<<weights_output[i][j]<<" ";
			
			for(int i = 0;i < number_hidden;i++)cout<<biases_hidden[i]<<" ";
			for(int i = 0;i < number_output;i++)cout<<biases_output[i]<<" ";
			cout<<endl;
		}
		bool test_file(){
			ifstream doc;
			doc.open(file_name);
			float temp;
			if(doc>>temp){
				doc.close();
				return true;
			}
			else{
				doc.close();
				return false;
			}
		}
		void calc_hidden(float *input,float *hidden){
			for(int i = 0;i < number_hidden;i++){
				hidden[i] = 0;
				for(int j = 0;j < number_input;j++)
					hidden[i] += input[j]*weights_hidden[j][i];
				hidden[i] += biases_hidden[i];
				activate_hidden(hidden[i]);
			}	
		}
		void activate_hidden(float &number){
			if(number < 0)number = 0;
			else number = 1;
			//cout<<number<<endl;
		}
		void calc_output(float *input,float *output){
			float *hidden = new float[number_hidden];
			calc_hidden(input,hidden);
			for(int i = 0;i < number_output;i++){
				output[i] = 0;
				for(int j = 0;j < number_hidden;j++)
					output[i] += hidden[j]*weights_output[j][i];
				output[i] += biases_output[i];
				activate_output(output[i]);
			}
			
			delete[] hidden;	
		}
		void activate_output(float &number){
			if(number < 0)number = 0;
			else number = 1;
		}
		
		void calc_error_output(float *input,float *answer,float *error){
			float *guess = new float[number_output];
			
			calc_output(input,guess);
			for(int i = 0;i < number_output;i++){
				error[i] = answer[i] - guess[i];
				//cout<<error[i]<<endl;
			}
			
			delete[] guess;
		}
		void calc_error_hidden(float *input,float *answer,float *error){
			float *error_output = new float[number_output];
			
			calc_error_output(input,answer,error_output);		
				
			for(int i = 0;i < number_hidden;i++){
				error[i] = 0;
				for(int j = 0;j < number_output;j++){
				//cout<<error[i]<<" += "<<error_output[j]<<"*"<<weights_output[i][j]<<endl;	
				error[i] += error_output[j]*weights_output[i][j];
				}
			}	
			
			delete[] error_output;
		}
		void teach(float *input,float *answer){
			float *hidden = new float[number_hidden];
			float *error_output = new float[number_output];
			float *error_hidden = new float[number_hidden];
			
			calc_hidden(input,hidden);
			calc_error_output(input,answer,error_output);
			calc_error_hidden(input,answer,error_hidden);
			
			for(int i = 0;i < number_output;i++)
				for(int j = 0;j < number_hidden;j++){
					//cout<<weights_hidden[j][i]<<" += "<<rate<<"*"<<error_output[i]<<"*"<<hidden[j]<<endl;
					weights_output[j][i] += rate*error_output[i]*hidden[j];
					}
					
			for(int i = 0;i < number_hidden;i++)
				for(int j = 0;j < number_input;j++){
					//cout<<weights_hidden[j][i]<<" += "<<rate<<"*"<<error_hidden[i]<<"*"<<input[j]<<endl;
					weights_hidden[j][i] += rate*error_hidden[i]*input[j];
					}
			
			for(int i = 0;i < number_hidden;i++)biases_hidden[i] += rate*error_hidden[i];
			for(int i = 0;i < number_output;i++)biases_output[i] += rate*error_output[i];
			
			delete[] hidden;
			delete[] error_output;
			delete[] error_hidden;			
		}
		int get_number_inputs(){
			return number_input;
		}
		int get_number_outputs(){
			return number_output;
		}
	};
		
void learn(brain &a){
	int number_inputs = a.get_number_inputs();
	int number_outputs = a.get_number_outputs();
	float *inputs = new float[number_inputs];
	float *outputs = new float[number_outputs];
	ifstream book;
	book.open("book.txt");
	while(book>>inputs[0]){
	cout<<inputs[0]<<"  ";
	for(int i = 1;i < number_inputs;i++){
		book>>inputs[i];
		cout<<inputs[i]<<"  ";
		}
	cout<<" = ";
	for(int i = 0;i < number_outputs;i++){
		book>>outputs[i];
		cout<<outputs[i]<<"  ";
	}
	cout<<endl<<"teaching..."<<endl;
	a.teach(inputs,outputs);
	}
	book.close();
	delete[] inputs;
	delete[] outputs;
}

void test(brain &a){
	int number_inputs = a.get_number_inputs();
	int number_outputs = a.get_number_outputs();
	float *inputs = new float[number_inputs];
	float *outputs = new float[number_outputs];
	ifstream book;
	book.open("test.txt");
	while(book>>inputs[0]){
	cout<<inputs[0]<<"  ";
	for(int i = 1;i < number_inputs;i++){
		book>>inputs[i];
		cout<<inputs[i]<<"  ";
		}
	cout<<" = ";
	a.calc_output(inputs,outputs);
	for(int i = 0;i < number_outputs;i++){
		cout<<outputs[i]<<"  ";
	}
	}
	book.close();
	delete[] inputs;
	delete[] outputs;
}

void manual_test(brain &a){
	int number_inputs = a.get_number_inputs();
	int number_outputs = a.get_number_outputs();
	float *inputs = new float[number_inputs];
	float *outputs = new float[number_outputs];
	for(int i = 0;i < number_inputs;i++)
		cin>>inputs[i];
	a.calc_output(inputs,outputs);
	for(int i = 0;i < number_outputs;i++)
		cout<<outputs[i]<<"  ";
	cout<<endl;
	delete[] inputs;
	delete[] outputs;
}
void manual_learn(brain &a){
	int number_inputs = a.get_number_inputs();
	int number_outputs = a.get_number_outputs();
	float *inputs = new float[number_inputs];
	float *outputs = new float[number_outputs];
	for(int i = 0;i < number_inputs;i++)
		cin>>inputs[i];
	for(int i = 0;i < number_outputs;i++)
		cin>>outputs[i];
	cout<<"teaching..."<<endl;
	a.teach(inputs,outputs);
	delete[] inputs;
	delete[] outputs;
}
		
int main(){
	brain a;
	int choise = 0;
	do
		{
		cout<<"Functions :"<<endl<<" 1 - Manual enter;"<<endl<<" 2 - Manual train"<<endl<<" 3 - Train from book"<<endl<<" 4 - Test from book"<<endl<<" 5 - exit"<<endl;
		cin>>choise;
		if(choise < 1 || choise > 5){cout<<"Wrong input please chose correct!"<<endl; continue;}
		if(choise == 1)
		{
			int number_inputs;
			cout<<"how many inputs :";
			cin>>number_inputs;
			for(int i = 0;i < number_inputs;i++)
				manual_test(a);
		}
		if(choise == 2)
		{
			int number_trains;
			cout<<"how many trains :";
			cin>>number_trains;
			for(int i = 0;i < number_trains;i++)
				manual_learn(a);
		}
		if(choise == 3)
		{
			int number_learns;
			cout<<"how many times to read : ";
			cin>>number_learns;
			for(int i = 0;i < number_learns;i++)
				learn(a);
		}	
		if(choise == 4)test(a);	
			
			
		}
	while(choise != 5);
	return 0;
}
