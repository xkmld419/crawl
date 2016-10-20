/*
* Copyright (c) 2011,��˾����
* All rights reserved.
* 
* �ļ����ƣ�stack.h 
* ժ    Ҫ���ļ�������stack ģ��
* 
*
* ȡ���汾��1.0 
* ԭ����  ��������
* ������ڣ�2011��11��10��
*/
#ifndef __STACK_H_
#define __STACK_H_

namespace __HSS_Stack {

	template<typename T>
	class Stack {
	public:
		Stack(unsigned int uiSize = 64) {
			size = uiSize;
			data = new T[size];

			itop = 0;
		}
		virtual ~Stack();

		int push(T& node);

		T& pop();

		T& top();
		
		int empty();

	protected:
		T*	data;
		unsigned int itop;
		unsigned int size;
	};

	template<typename T>
	Stack<T>::~Stack() {
		size = 0;

		if (data) {
		   delete data;
		}
		data = NULL;
		itop	= 0;
	}


	template<typename T>
	int Stack<T>::push(T& node) {
		if (itop >= size) {
			return -1;
		}

		data[itop] = node;
		itop++;

		return 0;
	}

	template<typename T>
	T& Stack<T>::pop() {
		if (itop <= 0) {
			return data[0];
		}

		return data[--itop];
	}

	template<typename T>
	T& Stack<T>::top() {
		if (top <= 0) {
			return data[0];
		}

		return data[itop - 1];
	}

	template<typename T>
	int Stack<T>::empty() {
		return itop ? 0 : 1;
	}

}

using namespace __HSS_Stack;
#endif

