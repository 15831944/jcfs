//ʵ�ֺ�����ƴ����ת��
//-----------------------------------------------------�����:ţ��ƽ
class CChiToLetter
{
public:
	CChiToLetter();
	~CChiToLetter();
	//�ָ���
	BOOL m_LetterEnd;
	//TRUE:�õ�����ĸ��д
	//FALSE:�õ�����ĸСд
	BOOL m_blnFirstBig;
	//TRUE:�õ�ȫ����д
	//FALSE:�õ���ȥ��ƴ��������Сд
	BOOL m_blnAllBiG;
	//True:�õ�ȫ��ƴ��
	//FALSE:�õ���ƴ��
	BOOL m_blnSimaple;
	//����ƴ��
	CString GetLetter(CStringA strText);
private:
	CString FindLetter(int nCode);
};