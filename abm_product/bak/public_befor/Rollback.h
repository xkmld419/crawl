/*VER: 1*/ 
#ifndef ROLLBACK_H
#define ROLLBACK_H

#define BUF_LEN	5000

class Rollback
{
  public:
	/*	����ĳ���������һЩ����	*/
	bool rollback (char *sTableName, char *sCondition);

  public:
	Rollback();

  private:
	void rollbackEventAggr (char *sTableName, char *sCondtion);
	void rollbackAcctAggr (char *sTableName, char *sCondtion);
	void rollbackEvent (char *sTableName, char *sCondition);
};

#endif
