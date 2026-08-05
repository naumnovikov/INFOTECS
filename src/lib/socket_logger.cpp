#include "socket_logger.hpp"
#include <vector>
#include <ctime>

namespace logging{
    inline constexpr int TCP_VALUE = 0;
    inline constexpr int EDGE_CASE = 0;
    using Byte = std::uint8_t;

    SocketLogger::SocketLogger(IpType ip, PortType port, LogLevel default_level){
        struct sockaddr_in addr;
        fd_opt = socket(AF_INET, SOCK_STREAM, TCP_VALUE);
        if(fd_opt < EDGE_CASE){
            std::cerr << "Can't create a socket.\n";
            return;
        }
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(ip);
        if(connect(fd_opt.value(), (struct sockaddr *)&addr, sizeof(addr)) < EDGE_CASE){
            std::cerr << "Can't connect.\n";
            closeSocket();
        }
    }

    inline constexpr int HEADERS_LENGTH = 9;
    inline constexpr int FIELDS_IN_TIMESTAMP = 6;
    inline constexpr int YEAR_SHIFT = 1900;
    inline constexpr int MONTH_SHIFT = 1;
    using ParsedTime = std::vector<Byte>;
    using DoubleByte = std::uint16_t;

    #pragma pack(push, 1) 
    struct MsgHeader {
        Byte  full_msg_size; 
        Byte  day;           
        Byte  month;       
        DoubleByte year;         
        Byte  hour;    
        Byte  min;      
        Byte  sec;           
        Byte  log_lvl;      
    };
    #pragma pack(pop)

    void setLocalTime(MsgHeader& header){
        std::time_t t = std::time(nullptr);
        std::tm* local_time = std::localtime(&t); 
        header.day = local_time->tm_mday;      
        header.month = local_time->tm_mon + MONTH_SHIFT;
        header.year = htons(local_time->tm_year + YEAR_SHIFT); 
        header.hour = local_time->tm_hour;        
        header.min = local_time->tm_min;         
        header.sec = local_time->tm_sec;
    }
    void fillMsgHeader(MsgHeader& header, std::size_t msg_length, LogLevel lvl){
        header.full_msg_size = msg_length + HEADERS_LENGTH;
        setLocalTime(header);
        header.log_lvl = static_cast<Byte>(lvl);
    }
    void SocketLogger::log(std::string_view msg, LogLevel lvl){
        if (!fd_opt.has_value()){
            std::cerr << "Socket is undefined.\n";
            return;
        }
        if (lvl < current_log_lvl){
            return;
        }
        MsgHeader header;
        std::size_t msg_length = msg.length(); 
        fillMsgHeader(header, msg_length, lvl);
        if (send(fd_opt.value(), reinterpret_cast<const void*>(&header), HEADERS_LENGTH, TCP_VALUE) < EDGE_CASE){
            std::cerr << "Can't send logs' header to socket.\n";
            return;
        }
        if (!msg.empty()){
            if (send(fd_opt.value(), msg.data(), msg_length, TCP_VALUE) < EDGE_CASE){
                std::cerr << "Can't send log to socket.\n";
            }
        }
    }
}