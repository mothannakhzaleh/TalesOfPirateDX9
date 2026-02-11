#pragma once

#ifndef _BTISERVICE_H_
#define _BTISERVICE_H_

class CBTI
{
public:
    CBTI();
    ~CBTI();

    bool Open(char const* ip1, char const* ip2);
    bool Close();

    long Start(char const* name, char const* passport);
    void Stop(char const* name);
    long GetData(char const* name, long v);

    long Buys(char const* name, long credit, char const* obj);
    long Sale(char const* name, long credit, char const* obj);

    long Deal(char const* p_name, char const* m_name, long credit, char const* obj);

    long Give(char const* name, long credit, char const* svc);
    long Bill(char const* name, long credit, char const* svc);

private:
    bool m_openflag;
};

#endif