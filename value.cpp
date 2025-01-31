#include<iostream>
#include <set>
#include <functional>  
#include <unordered_set>
#include <list>
#include <stack>
#include<math.h>

class Value {
    // public data members
    public:
        double data;
        double grad;

    // private variables used for autograd graph construction
    private:
    std::function<void()> _backward;
    std::set<Value*> _prev;
    std::string _op;

    public:
        // Define constructor
        Value(double data, std::initializer_list<Value*> _children = {}, std::string _op = ""){
            this->data = data;
            this->grad = 0;
            this->_backward = [](){};
            this->_op = _op;
            // use initializer list to assign values to _prev 
            this->_prev = std::set<Value*>(_children);
        }

        // Define destructor
        ~Value(){
            // No explicit cleanup is needed!
        }

        // Overload << operator for representing Value object
        friend std::ostream& operator<<(std::ostream& os, const Value& v){
            os<<"Value(data="<<v.data<<", grad="<<v.grad<<", op='"<<v._op<<"')";
            return os;
        }

        // Display children
        void displayPrev(){
            std::cout<<"Prev of Value(data="<<this->data<<"):"<<std::endl;
            for(auto child : this->_prev){
                std::cout<<"\t"<<*child<<std::endl;
            }
        };

        // Operator overloading for addition
        Value operator+(Value& other) {
            Value out(this->data + other.data, 
                {this, &other}, 
                "+");
            out._backward = [this, &other, &out]() {
                this->grad += out.grad;
                other.grad += out.grad;
            };

            return out;
        };

        // Operator overloading for multiplication
        Value operator*(Value& other){
            Value out(this->data*other.data,
                {this, &other},
                "*");
            out._backward = [this, &other, &out](){
                this->grad += other.data*out.grad;
                other.grad += this->data*out.grad;
            };

            return out;
        } 

        // Operator overloading for power
        Value operator^(double other){
            Value out(std::pow(this->data, other),
            {this},
            "^");
            out._backward = [this, other, &out](){
                this->grad += (other * std::pow(this->data, other-1))*out.grad;
            };

            return out;
        }

        // ReLU pass
        Value relu(){
            Value out{this->data<0?0:this->data,
                {this},
                "ReLU"};

            out._backward = [this, &out](){
                this->grad += (out.data > 0)*out.grad;
            };
        
            return out;
        }

        // backward
        void backward(){
            std::list<Value*> topo;
            std::unordered_set<Value*> visited;
            // std::stack<Value*> stack;

            auto build_topo = [&](Value* v){
                // COMPLETE!
            };

        };

};


int main(){
    Value a(2.0);
    Value b(3.0);
    Value c = a+b;
    Value d = a*b;
    Value e = d*c;
    Value f = e.relu();
    Value g = (f.relu() + d.relu())*b;
    std::cout << c << std::endl;
    std::cout << d << std::endl;
    std::cout << e << std::endl;
    std::cout << f << std::endl;
    g.displayPrev();
    
    
    
    return 0;
}