#ifndef __NEWVECTOR_H__
#define __NEWVECTOR_H__

//vector�ͷŵķ�ֵ
#define FREE_SIZE 80000

#include <vector>
using  namespace std;

template <class T>
class newVector : public vector<T>
{
	public:
		newVector()
		{
			lCurrentStartPosition = 0;
			lSize = 0;
			lSumElementCount = 0;
		}

		void clear()
		{
			lCurrentStartPosition = 0;
			lSize = 0;
			lSumElementCount = 0;
			vector<T>::clear();
		}

		void push_back(const T &X)
		{
			vector<T>::push_back(X);
			lCurrentStartPosition++;
			lSize++;
			lSumElementCount++;
		}

		bool getElement(T &X)
		{
			if( lCurrentStartPosition < lSumElementCount )//��Ԫ�ؿ���
			{
				lCurrentStartPosition++;
				lSize++;
				X = (*this)[lCurrentStartPosition-1];
				return true;
			}
			else                                          //û��Ԫ�ؿ���
			{
				X = NULL;
				return false;
			}
		}

    void reset()//���ڴ�ͷ��ʼʹ��Ԫ��
    {
			lCurrentStartPosition = 0;
			lSize = 0;
			if( lSumElementCount > FREE_SIZE )
			{
				for(int i=0;i<lSumElementCount;i++)
				{
					delete (*this)[i];
				}
				clear();
			}
		}
    void focusReset()//for select use zhu
    {
				lCurrentStartPosition = 0;
				lSize = 0;
				for(int i=0;i<lSumElementCount;i++)
				{
					delete (*this)[i];
				}
				clear();
	}

		long getSumElementCount()//�õ�vector�д�ŵ�Ԫ�ظ�����Ϊ�ͷ�����vector����
		{
			return lSumElementCount;		
		}

		long size()
		{
			return lSize;
		}

	private:
		long lCurrentStartPosition;  //���ο�ʼ���±�
		long lSize;
		long lSumElementCount;       //vector�д�ŵ�Ԫ�ظ���
		
};

#endif
