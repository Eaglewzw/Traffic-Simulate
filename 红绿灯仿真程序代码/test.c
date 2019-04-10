#include<reg51.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int
	
uchar code table1[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77}; //数字0-9和初始化字符
uchar num1=0,num2=0;//num1为主干道计数值，num2为次干道计数值

//用int是因为uchar的范围为0-255。
int MainFigure=47;//主干道数字初始化(以红灯开始)
uchar SecondaryFigure=50;//次干道初始化(以绿灯开始)
uchar MainTemp=47;//主干道数字修改变量
uchar SecondaryTemp=50;//次干道修改变量
//flag处于1、2、3依次表示处于红黄绿不同的状态
uchar flag1=0,flag2=0;//状态1标志位(主干道);状态2标志位(次干道)
uchar ChooseFlag=0;//ChooseFlag选中标志位

sbit dula=P2^6;//段选
sbit wela=P2^7;//位选
sbit MainSideWalkRed=P1^0;//主干道人行道红灯
sbit MainSideWalkGreen=P1^1;//主干道人行道绿灯
sbit SecondaryWalkRed=P3^6;//次干道人行道红灯
sbit SecondaryWalkGreen=P3^7;//次干道人行道绿灯
sbit MainRed=P2^0;//主干道红灯
sbit MainGreen=P2^1;//主干道绿灯
sbit MainYellow=P2^2;//主干道黄灯
sbit SecondaryRed=P2^3;//次干道红灯
sbit SecondaryGreen=P2^4;//次干道绿灯
sbit SecondaryYellow=P2^5;//次干道黄灯
sbit KeySet=P1^2;//设置按键
sbit TimeUp=P1^3;//加键
sbit TimeDown=P1^4;//减键
/*
	代码说明：
	红绿灯分为主干道和次干道，主次干道时间红绿灯时间是相同的。
	本代码不涉及转向灯的设计部分。具体说明如下：
	初始化后：
	主干道 绿灯30s  红灯47s 黄灯3s(固定)
  	次干道 绿灯50s  红灯27s 黄灯3s(固定)
	程序中本来总的时间为80s,通过按键只可修改主干道红灯，次干道绿灯的值，且一旦主道红灯确定
	次干道绿灯的值也就确定了，所以只要通过按键修改一个干道的时间即可。
	在这里主干道和次干道只是相对概念，因实际放置位置而分别确定。
	
*/



void delay_ms(int z); //延时函数
void TimeInit();//定时器初始化
void NixieTubeDisplay();//所有数码管显示
void LEDfunction();//主次干道红绿灯显示
void TrafficTimeSet();

int main(void)
{
 	uchar task=0;
	TimeInit();

	while(1)
	{	
		
		TrafficTimeSet();
			
		NixieTubeDisplay();
		LEDfunction();//主次干道红绿灯显示	
			
	}  		  
}

void TrafficTimeSet()
{
		if(KeySet==0)//如果设置按键按下
		{
			delay_ms(10);
			if(KeySet==0)
			{
				while(!KeySet);//等待按键弹起
				//全黄
				
				//如果根本没按加键减
				
				ChooseFlag++;
				if(ChooseFlag==1)
				{
					MainRed=MainGreen=1;
					MainYellow=0;
					SecondaryRed=SecondaryGreen=1;
					SecondaryYellow=0;
					MainSideWalkRed=MainSideWalkGreen=SecondaryWalkRed=SecondaryWalkGreen=1;
					num1=num2=0;flag1=0;flag2=0;
					TR0=0;//关闭定时器0
					MainFigure=0;//修改主干道
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
						MainTemp=MainFigure;//保存变量
					}
					else
					{						
						SecondaryFigure=MainFigure+3;//修改的主干道红灯最终结果
						SecondaryTemp=SecondaryFigure;
						MainTemp=MainFigure;//保存变量
					}

				}
				
			}
		}
		if(TimeUp==0)
		{
			delay_ms(10);
			if(TimeUp==0)
			{
				while(!TimeUp);//等待按键弹起

				if(ChooseFlag==1)
				{
					MainFigure++;
					if(MainFigure==97)
					{
						MainFigure=47;//最大96,到96回到默认值47
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
				while(!TimeDown);//等待按键弹起
	
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
	EA=1;//打开总中断
	ET0=1;//打开定时器0中断
    TR0=1;//启动定时器0
}

void Timer() interrupt 1
{
 	TH0=(65535-45872)/256;
	TL0=(65535-45872)%256;
	num1++;num2++;
	if(num1==20)//1秒到了
	{
		num1=0;
		MainFigure--;SecondaryFigure--;
		if(MainFigure==0)
		{
			flag1++;
			switch(flag1)
			{
			 	case 1:MainFigure=3;break;//(标志位1)3s黄灯
				case 2:MainFigure=30;break;//(标志位2)30s绿灯
				case 3:MainFigure=MainTemp;flag1=0;break;//(标志位3)45s红灯，回到起始位
			}		
			
		}	
		if(SecondaryFigure==0)
		{
			flag2++;
			switch(flag2)
			{
				case 1:SecondaryFigure=27;break;//(标志位1)27s红灯
				case 2:SecondaryFigure=3;break;//(标志位2)3s黄灯
				case 3:SecondaryFigure=SecondaryTemp;flag2=0;break;//(标志位3)48s绿灯,回到起始位
			}			   
		}  
	}
	
	if(num2==11)
	{
	 	num2=0;
		if((flag1==1)&&(MainFigure>=1&&MainFigure<=3))//固定黄灯灯为3秒
		{
			MainRed=1;
			MainGreen=1;
			MainYellow=~MainYellow;

			MainSideWalkRed=1;
			MainSideWalkGreen=0;//主干道人行道绿灯


		}//主干道黄灯
		if((flag2==2)&&(SecondaryFigure>=1&&SecondaryFigure<=3))//固定黄灯
		{
			SecondaryRed=1;
			SecondaryGreen=1;
			SecondaryYellow=~SecondaryYellow;
			SecondaryWalkRed=1;
			SecondaryWalkGreen=0;//次干道人行道绿灯
		}//次干道黄灯
		
	
	} 

}

void delay_ms(int z)
{
	int x,y;
	for(x=z;x>0;x--)
	for(y=110;y>0;y--);
}

void NixieTubeDisplay()//所有数码管显示
{
	int bai1,shi1;
	int bai2,shi2;
	bai1=MainFigure/10;
	shi1=MainFigure%10;
	bai2=SecondaryFigure/10;
	shi2=SecondaryFigure%10;//分离数字

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
	delay_ms(3);//主干道数码管	  

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
	delay_ms(3);//次干道 
}

void LEDfunction()//主次干道红绿灯显示
{
	if(ChooseFlag==0)
	{
		if((flag1==0||flag1==3)&&(MainFigure>=1&&MainFigure<=MainTemp))
		{
			MainRed=0;
			MainYellow=1;
			MainGreen=1;

			MainSideWalkRed=1;
			MainSideWalkGreen=0;//主干道人行道绿灯


		}//主干道红灯

		if((flag1==2)&&(MainFigure>=1&&MainFigure<=30))
		{
			MainRed=1;
			MainYellow=1;
			MainGreen=0;

			MainSideWalkGreen=1;
			MainSideWalkRed=0;


		}//主干道绿灯	 


		if((flag2==0||flag2==3)&&(SecondaryFigure>=1&&SecondaryFigure<=SecondaryTemp))
		{
			SecondaryRed=1;
			SecondaryGreen=0;
			SecondaryYellow=1;

			SecondaryWalkRed=0;//次干道人行道红灯
			SecondaryWalkGreen=1;


		}//次干道绿灯
		if((flag2==1)&&(SecondaryFigure>=1&&SecondaryFigure<=27))
		{
			SecondaryRed=0;
			SecondaryGreen=1;
			SecondaryYellow=1;

			SecondaryWalkRed=1;
			SecondaryWalkGreen=0;//次干道人行道绿灯

		}//次干道红灯灯	 
	}

}