#ifndef ERROR_CODE_H
#define ERROR_CODE_H

struct ErrorCode
{
    ErrorCode(int code, QString const& msg = "") : code(code), msg(msg) {}

    enum {
      Success,
      FileNotFound,
      ReadError
    };

    int code;
    QString msg;
};

#endif // ERROR_CODE_H
