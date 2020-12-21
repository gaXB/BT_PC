#include "stdafx.h"
//#include "ToolBar.h"
#include "EepromDialog1.h"
#include"StdioFileEx.h"

//吹面电压设置值：m_osf,。。。。和 rxdata_int【】的对应 rxdata_int【n-1】=eeprom【n】（eeprom为mcu）
void CEepromDialog::RefshMgrid()
{
	 m_osf= rxdata_int[1]*256+rxdata_int[2];
	 m_osfd= rxdata_int[3]*256+rxdata_int[4];
	 m_osd= rxdata_int[5]*256+rxdata_int[6];
	 m_osdt= rxdata_int[7]*256+rxdata_int[8];
	 m_ost= rxdata_int[9]*256+rxdata_int[10];
	 m_osf=m_osf/5;
	 m_osfd=m_osfd/5;
	 m_osd=m_osd/5;
	 m_osdt=m_osdt/5;
	 m_ost=m_ost/5;


	// m_osf=m_osf/51;
	//m_osfd=m_osfd/51;
	// m_osd=m_osd/51;
	// m_osdt=m_osdt/51;
	// m_ost=m_ost/51;

	 m_feseat_1=rxdata_int[12];
	 m_feseat_2=rxdata_int[13];
	 m_feseat_3=rxdata_int[14];
	 m_feseat_4=rxdata_int[15];
	 m_feseat_5=rxdata_int[16];
	 m_feseat_6=rxdata_int[17];
	 m_feseat_7=rxdata_int[18];
	 m_feseat_8=rxdata_int[19];


	 m_maxhi=rxdata_int[20]*256+rxdata_int[21];
	 m_maxlo=0;
	 m_maxhi=m_maxhi/5;

	 m_Ktset=rxdata_int[22]/10;
	 m_Ktin=rxdata_int[23]/-10;

	 if(rxdata_int[24]>=128)
	 {
		m_Kamb=rxdata_int[24]-256;
	 }
	 else	m_Kamb=rxdata_int[24];
	 m_Kamb=m_Kamb/10;
	 
	 if(rxdata_int[25]>=128)
	 {
		m_Ksun=rxdata_int[25]-256;
	 }
	 else	m_Ksun=rxdata_int[25];
	 m_Ksun=m_Ksun/10;

	 m_offset=rxdata_int[26]*256+rxdata_int[27];	
	 UpdateData(FALSE);
}




/**************************************************
void CEepromDialog::Read_Ed()

由设置值转化为  rxdata【】 并刷入mcu 

*****************************************************/

int	Mode_Rd(float m_os_a)
{
	m_os_a=5*m_os_a;
	if(m_os_a>1800) return 1800;
	if(m_os_a<0)  return 0;

	return (int)m_os_a;

}

int CEepromDialog::Read_Ed()
{
	for(int i=0;i<Max_N_Stat;i++) rxdata_int[i]=0;
	UpdateData(TRUE);
	//模式定义
	int m_osf_int=0,m_osfd_int=0,m_osd_int=0,m_osdt_int=0,m_ost_int=0,m_maxhi_int=0;
	m_osf_int=Mode_Rd(m_osf);
	m_osfd_int=Mode_Rd(m_osfd);
	m_osd_int=Mode_Rd(m_osd);
	m_osdt_int=Mode_Rd(m_osdt);
	m_ost_int=Mode_Rd(m_ost);
	m_maxhi_int=Mode_Rd(m_maxhi);

	
	rxdata_int[1] = m_osf_int/256;
	rxdata_int[2] = m_osf_int%256;

	rxdata_int[3] = m_osfd_int/256;
	rxdata_int[4] = m_osfd_int%256;

	rxdata_int[5]=m_osd_int/256;
	rxdata_int[6]=m_osd_int%256;

	rxdata_int[7]=m_osdt_int/256;
	rxdata_int[8]=m_osdt_int%256;

	rxdata_int[9]=m_ost_int/256;
	rxdata_int[10]=m_ost_int%256;


	rxdata_int[20]=m_maxhi_int/256;
	rxdata_int[21]=m_maxhi_int%256;



	//风挡定义
    rxdata_int[12]=m_feseat_1;
	rxdata_int[13]=m_feseat_2;
	rxdata_int[14]=m_feseat_3;
	rxdata_int[15]=m_feseat_4;
	rxdata_int[16]=m_feseat_5;
	rxdata_int[17]=m_feseat_6;
	rxdata_int[18]=m_feseat_7;
	rxdata_int[19]=m_feseat_8;


	//系数
	if((m_Ktset<25.6&&m_Ktset>0)&&(m_Ktin<0&&m_Ktin>-25.6)&&(m_Ksun>-12&&m_Ksun<12)&&(m_Kamb>-12&&m_Kamb<12))
	{
		rxdata_int[22]=(int)m_Ktset*10;


		rxdata_int[23]=(int)m_Ktin*-10;
		if(m_Kamb>=0)
		{
			rxdata_int[24]=m_Kamb*10;
		}
		else
			rxdata_int[24]=256-(int)(m_Kamb*-10);

		if(m_Ksun>=0)
		{
			rxdata_int[25]=m_Ksun*10;
		}
		else
			rxdata_int[25]=256-(int)(m_Ksun*-10);

		
		rxdata_int[26]=m_offset/256;
		rxdata_int[27]=m_offset%256;

	    return 0;
	}else return 1;
}


//初始值设置。重设时调用
//	  eeprom[10]= 0;eeprom[11]=  95;eeprom[12]=  120;eeprom[13]= 145;eeprom[14]= 170;eeprom[15]= 190;eeprom[16]=220;eeprom[17]=255;
void CEepromDialog::OnBnClickedButtonResetEe()
{
	// TODO: 在此添加控件通知处理程序代码
	m_feseat_1 = 95;
	m_feseat_2 = 116;
	m_feseat_3 = 137;
	m_feseat_4 = 158;
	m_feseat_5 = 179;
	m_feseat_6 = 201;
	m_feseat_7 = 220;

	m_feseat_8= 250;

	m_osf = 0;
	m_osfd = 153.0/5;
	m_osd = 307.0/5;
	m_osdt =461.0/5;
	m_ost =614.0/5;
	m_maxhi=1423.0/5;
	m_maxlo=0;

	m_offset=130;
	m_Kamb=0.5;
	m_Ktset=16;
	m_Ktin=-16;
    m_Ksun=-0.1;
	UpdateData(FALSE);
	Read_Ed();
}