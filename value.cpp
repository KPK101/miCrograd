#include<iostream>
#include <set>
#include <functional>  
#include <unordered_set>
#include <list>
#include <stack>


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
                
            };

        };



};
