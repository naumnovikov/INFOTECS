#include "socket_logger.hpp"
#include <vector>

namespace logging{
    inline constexpr int TCP_VALUE = 0;
    inline constexpr int EDGE_CASE = 0;
    using Byte = std::uint8_t;

    // In case init wasn't used before logging,
    // it just wouldn't log to socket because 
    // !fd_opt.has_value() condition returns true
    ErrorCode SocketLogger::init(IpType ip, PortType port, LogLevel default_level){
        struct sockaddr_in addr;
        fd_opt = socket(AF_INET, SOCK_STREAM, TCP_VALUE);
        if(fd_opt < EDGE_CASE){
            std::cerr << "Can't create a socket.\n";
            return 4;
        }
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(ip);
        if(connect(fd_opt.value(), (struct sockaddr *)&addr, sizeof(addr)) < EDGE_CASE){
            std::cerr << "Can't connect.\n";
            close();
            return 5;
        }
        return 0;
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

    void setLocalTime(MsgHeader& header) {
        std::time_t t = std::time(nullptr);
        std::tm local_time; 
        localtime_r(&t, &local_time); 
        header.day = local_time.tm_mday;      
        header.month = local_time.tm_mon + MONTH_SHIFT;
        header.year = htons(local_time.tm_year + YEAR_SHIFT); 
        header.hour = local_time.tm_hour;        
        header.min = local_time.tm_min;         
        header.sec = local_time.tm_sec;
    }
    void fillMsgHeader(MsgHeader& header, std::size_t msg_length, LogLevel lvl){
        header.full_msg_size = msg_length + HEADERS_LENGTH;
        setLocalTime(header);
        header.log_lvl = static_cast<Byte>(lvl);
    }
    ErrorCode SocketLogger::log(std::string_view msg, LogLevel lvl){
        if (!fd_opt.has_value()){
            std::cerr << "Socket is undefined.\n";
            return 1;
        }
        if (lvl < current_log_lvl){
            return 0;
        }
        MsgHeader header;
        std::size_t msg_length = msg.length(); 

        // In case program got logging message 
        // longer than 246 bytes (not fitting 1B field),
        // it will send only 246 bytes
        if (msg_length > 246){
            msg_length = 246;
        }
        fillMsgHeader(header, msg_length, lvl);
        std::lock_guard<std::mutex> lg(mtx);
        if (send(fd_opt.value(), reinterpret_cast<const void*>(&header), HEADERS_LENGTH, TCP_VALUE) < EDGE_CASE){
            std::cerr << "Can't send logs' header to socket.\n";
            return 2;
        }
        if (!msg.empty()){
            if (send(fd_opt.value(), msg.data(), msg_length, TCP_VALUE) < EDGE_CASE){
                std::cerr << "Can't send log to socket.\n";
            }
            return 2;
        }
        return 0;
    }
} // namespace logging