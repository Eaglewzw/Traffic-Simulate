#include<reg51.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int
	
uchar code table1[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77}; //����0-9�ͳ�ʼ���ַ�
uchar num1=0,num2=0;//num1Ϊ���ɵ�����ֵ��num2Ϊ�θɵ�����ֵ

//��int����Ϊuchar�ķ�ΧΪ0-255��
int MainFigure=47;//���ɵ����ֳ�ʼ��(�Ժ�ƿ�ʼ)
uchar SecondaryFigure=50;//�θɵ���ʼ��(���̵ƿ�ʼ)
uchar MainTemp=47;//���ɵ������޸ı���
uchar SecondaryTemp=50;//�θɵ��޸ı���
//flag����1��2��3���α�ʾ���ں���̲�ͬ��״̬
uchar flag1=0,flag2=0;//״̬1��־λ(���ɵ�);״̬2��־λ(�θɵ�)
uchar ChooseFlag=0;//ChooseFlagѡ�б�־λ

sbit dula=P2^6;//��ѡ
sbit wela=P2^7;//λѡ
sbit MainSideWalkRed=P1^0;//���ɵ����е����
sbit MainSideWalkGreen=P1^1;//���ɵ����е��̵�
sbit SecondaryWalkRed=P3^6;//�θɵ����е����
sbit SecondaryWalkGreen=P3^7;//�θɵ����е��̵�
sbit MainRed=P2^0;//���ɵ����
sbit MainGreen=P2^1;//���ɵ��̵�
sbit MainYellow=P2^2;//���ɵ��Ƶ�
sbit SecondaryRed=P2^3;//�θɵ����
sbit SecondaryGreen=P2^4;//�θɵ��̵�
sbit SecondaryYellow=P2^5;//�θɵ��Ƶ�
sbit KeySet=P1^2;//���ð���
sbit TimeUp=P1^3;//�Ӽ�
sbit TimeDown=P1^4;//����
/*
	����˵����
	���̵Ʒ�Ϊ���ɵ��ʹθɵ������θɵ�ʱ����̵�ʱ������ͬ�ġ�
	�����벻�漰ת��Ƶ���Ʋ��֡�����˵�����£�
	��ʼ����
	���ɵ� �̵�30s  ���47s �Ƶ�3s(�̶�)
  	�θɵ� �̵�50s  ���27s �Ƶ�3s(�̶�)
	�����б����ܵ�ʱ��Ϊ80s,ͨ������ֻ���޸����ɵ���ƣ��θɵ��̵Ƶ�ֵ����һ���������ȷ��
	�θɵ��̵Ƶ�ֵҲ��ȷ���ˣ�����ֻҪͨ�������޸�һ���ɵ���ʱ�伴�ɡ�
	���������ɵ��ʹθɵ�ֻ����Ը����ʵ�ʷ���λ�ö��ֱ�ȷ����
	
*/



void delay_ms(int z); //��ʱ����
void TimeInit();//��ʱ����ʼ��
void NixieTubeDisplay();//�����������ʾ
void LEDfunction();//���θɵ����̵���ʾ
void TrafficTimeSet();

int main(void)
{
 	uchar task=0;
	TimeInit();

	while(1)
	{	
		
		TrafficTimeSet();
			
		NixieTubeDisplay();
		LEDfunction();//���θɵ����̵���ʾ	
			
	}  		  
}

void TrafficTimeSet()
{
		if(KeySet==0)//������ð�������
		{
			delay_ms(10);
			if(KeySet==0)
			{
				while(!KeySet);//�ȴ���������
				//ȫ��
				
				//�������û���Ӽ���
				
				ChooseFlag++;
				if(ChooseFlag==1)
				{
					MainRed=MainGreen=1;
					MainYellow=0;
					SecondaryRed=SecondaryGreen=1;
					SecondaryYellow=0;
					MainSideWalkRed=MainSideWalkGreen=SecondaryWalkRed=SecondaryWalkGreen=1;
					num1=num2=0;flag1=0;flag2=0;
					TR0=0;//�رն�ʱ��0
					MainFigure=0;//�޸����ɵ�
					SecondaryFigure=0;
				}
				if(ChooseFlag==2)
				{
					TR0=1;ChooseFlag=0;
					if(MainFigure==0)
					{
						MainFigure=47;
						SecondaryFigure=50;
						SecondaryTemp=SecondaryFigure;
						MainTemp=MainFigure;//�������
					}
					else
					{						
						SecondaryFigure=MainFigure+3;//�޸ĵ����ɵ�������ս��
						SecondaryTemp=SecondaryFigure;
						MainTemp=MainFigure;//�������
					}

				}
				
			}
		}
		if(TimeUp==0)
		{
			delay_ms(10);
			if(TimeUp==0)
			{
				while(!TimeUp);//�ȴ���������

				if(ChooseFlag==1)
				{
					MainFigure++;
					if(MainFigure==97)
					{
						MainFigure=47;//���96,��96�ص�Ĭ��ֵ47
						SecondaryFigure=50;
					}
					else
					SecondaryFigure=MainFigure+3;
				
				}
			}
		}
		if(TimeDown==0)
		{
			delay_ms(10);
			if(TimeDown==0)
			{
				while(!TimeDown);//�ȴ���������
	
				if(ChooseFlag==1)
				{
					MainFigure--;
					if(MainFigure<0)
					{
						MainFigure=47;
						SecondaryFigure=50;
					}
					else
					SecondaryFigure=MainFigure+3;
			
				}
			}
		}
		
	

}
	
	
	
void TimeInit()
{
	TMOD=0x01;
	TH0=(65535-45872)/256;
	TL0=(65535-45872)%256;
	EA=1;//�����ж�
	ET0=1;//�򿪶�ʱ��0�ж�
    TR0=1;//������ʱ��0
}

void Timer() interrupt 1
{
 	TH0=(65535-45872)/256;
	TL0=(65535-45872)%256;
	num1++;num2++;
	if(num1==20)//1�뵽��
	{
		num1=0;
		MainFigure--;SecondaryFigure--;
		if(MainFigure==0)
		{
			flag1++;
			switch(flag1)
			{
			 	case 1:MainFigure=3;break;//(��־λ1)3s�Ƶ�
				case 2:MainFigure=30;break;//(��־λ2)30s�̵�
				case 3:MainFigure=MainTemp;flag1=0;break;//(��־λ3)45s��ƣ��ص���ʼλ
			}		
			
		}	
		if(SecondaryFigure==0)
		{
			flag2++;
			switch(flag2)
			{
				case 1:SecondaryFigure=27;break;//(��־λ1)27s���
				case 2:SecondaryFigure=3;break;//(��־λ2)3s�Ƶ�
				case 3:SecondaryFigure=SecondaryTemp;flag2=0;break;//(��־λ3)48s�̵�,�ص���ʼλ
			}			   
		}  
	}
	
	if(num2==11)
	{
	 	num2=0;
		if((flag1==1)&&(MainFigure>=1&&MainFigure<=3))//�̶��ƵƵ�Ϊ3��
		{
			MainRed=1;
			MainGreen=1;
			MainYellow=~MainYellow;

			MainSideWalkRed=1;
			MainSideWalkGreen=0;//���ɵ����е��̵�


		}//���ɵ��Ƶ�
		if((flag2==2)&&(SecondaryFigure>=1&&SecondaryFigure<=3))//�̶��Ƶ�
		{
			SecondaryRed=1;
			SecondaryGreen=1;
			SecondaryYellow=~SecondaryYellow;
			SecondaryWalkRed=1;
			SecondaryWalkGreen=0;//�θɵ����е��̵�
		}//�θɵ��Ƶ�
		
	
	} 

}

void delay_ms(int z)
{
	int x,y;
	for(x=z;x>0;x--)
	for(y=110;y>0;y--);
}

void NixieTubeDisplay()//�����������ʾ
{
	int bai1,shi1;
	int bai2,shi2;
	bai1=MainFigure/10;
	shi1=MainFigure%10;
	bai2=SecondaryFigure/10;
	shi2=SecondaryFigure%10;//��������

	wela=1;
	P0=0xBE;//1011 1110
	wela=0;
	dula=1;
	P0=table1[bai1];
	dula=0;
	P0=0xff;
	delay_ms(3);
	
	wela=1;
	P0=0x7D;//0111 1101
	wela=0;
	dula=1;
	P0=table1[shi1];	 
	dula=0;
	P0=0xff;
	delay_ms(3);//���ɵ������	  

	wela=1;
	P0=0xEB;//1110 1011
	wela=0;
	dula=1;
	P0=table1[bai2];
	dula=0;
	P0=0xff;
	delay_ms(3); 

	wela=1;
	P0=0xD7;//1101 0111
	wela=0;
	dula=1;
	P0=table1[shi2];	 
	dula=0;
	P0=0xff;
	delay_ms(3);//�θɵ� 
}

void LEDfunction()//���θɵ����̵���ʾ
{
	if(ChooseFlag==0)
	{
		if((flag1==0||flag1==3)&&(MainFigure>=1&&MainFigure<=MainTemp))
		{
			MainRed=0;
			MainYellow=1;
			MainGreen=1;

			MainSideWalkRed=1;
			MainSideWalkGreen=0;//���ɵ����е��̵�


		}//���ɵ����

		if((flag1==2)&&(MainFigure>=1&&MainFigure<=30))
		{
			MainRed=1;
			MainYellow=1;
			MainGreen=0;

			MainSideWalkGreen=1;
			MainSideWalkRed=0;


		}//���ɵ��̵�	 


		if((flag2==0||flag2==3)&&(SecondaryFigure>=1&&SecondaryFigure<=SecondaryTemp))
		{
			SecondaryRed=1;
			SecondaryGreen=0;
			SecondaryYellow=1;

			SecondaryWalkRed=0;//�θɵ����е����
			SecondaryWalkGreen=1;


		}//�θɵ��̵�
		if((flag2==1)&&(SecondaryFigure>=1&&SecondaryFigure<=27))
		{
			SecondaryRed=0;
			SecondaryGreen=1;
			SecondaryYellow=1;

			SecondaryWalkRed=1;
			SecondaryWalkGreen=0;//�θɵ����е��̵�

		}//�θɵ���Ƶ�	 
	}

}