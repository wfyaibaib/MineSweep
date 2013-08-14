#ifndef BLOCK_H_
#define BLOCK_H_

#include<QLabel>
class QWidget;

class Block:public QLabel
{
	Q_OBJECT

public:
    // ���� �Ƿ�����
	explicit Block(bool mine_flag,QWidget* parent=0);
    // ������Χ����
	void set_number(int number);
    // ��ת
	void turn_over();
    // �Ƿ�����
	bool is_mine()const;
    // �Ƿ��Ѿ�������
	bool is_turn_over()const;
	
signals:
    // �������ź�
	void turn_over(bool is_mine);
	
protected:
    // �����
	void mousePressEvent(QMouseEvent* event);
	
private:
    // �Ƿ�����
	bool mine_flag_;
    // �Ƿ��б��
	bool mark_flag_;
    // �Ƿ񱻷���
	bool turn_over_flag_;
    // ��Χ����
	int number_;
};

#endif
