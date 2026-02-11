
#ifndef BILL_H_
#define BILL_H_

int PSS_GetCredit(char const* section, char const* name, std::string& strErr);

class CPAI {
public:
    CPAI();
    ~CPAI() {}

    int Login(char const* name, char const* pwd);       //  修改返回值，10000成功，10001失败，10004连接失败
    std::string GetPassport();

    std::string LastError();    //  add by jampe

private:
};

#endif