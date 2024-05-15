#include <boost/redis/config.hpp>
#include <boost/redis/connection.hpp>
//#include <boost/asio.hpp>
//#include <boost/asio/detached.hpp>

#include <iostream>

//namespace asio = boost::asio;
//using boost::redis::connection;
//using boost::redis::request;
//using boost::redis::response;
//using boost::redis::config;

int main()
{
    try {
        boost::redis::config cfg;
        cfg.use_ssl = false;
        cfg.addr.host = "127.0.0.1";
        cfg.addr.port = 6379;

  
        //boost::redis::request req;
        //req.push("PING", "Hello world");


        //boost::redis::response<std::string> resp;
        /*
        asio::io_context ioc;
        boost::redis::connection conn{ ioc };
        conn.async_run(cfg, {}, asio::detached);

        conn.async_exec(req, resp, [&](auto ec, auto) {
            if (!ec)
                std::cout << "PING: " << std::get<0>(resp).value() << std::endl;
            conn.cancel();
            });

        ioc.run();*/

    }
    catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
