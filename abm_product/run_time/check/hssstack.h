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
#ifndef __HSS_STACK_H_
#define __HSS_STACK_H_


template<typename T>
class HSS_Stack {
public:
	HSS_Stack(unsigned int uiSize = 64) {
		size = uiSize;
		data = new T[size];

		itop = 0;
	}
	virtual ~HSS_Stack(){
		size = 0;

		if (data) {
		   delete data;
		}
		data = NULL;
		itop	= 0;
	}

	int push(T& node){
		if (itop >= size) {
			return -1;
		}

		data[itop] = node;
		itop++;

		return 0;
	}

	T& pop() {
		if (itop <= 1) {
			return data[0];
		}
	
		return data[--itop];
	}
	

	T& top() {
		if (itop <= 0) {
			return data[0];
		}

		return data[itop - 1];
	}

	int low() {
		return itop > 1 ? 1 : 0;
	}
	
	int empty() {
		return itop ? 0 : 1;
	}
	

protected:
	T*	data;
	unsigned int itop;
	unsigned int size;
};

#endif

