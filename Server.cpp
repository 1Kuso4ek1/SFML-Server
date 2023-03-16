#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <memory>

int main()
{
	sf::SocketSelector selector;
	sf::IpAddress ip = sf::IpAddress::getPublicAddress();
	sf::TcpListener listener;
	std::vector<std::shared_ptr<sf::TcpSocket>> sockets;
	
	int port;
	std::cout << "Enter port: ";
	std::cin >> port;

	listener.listen(port);
	listener.setBlocking(false);
	selector.add(listener);

	std::cout << "Server started! IP Adress: " << ip << std::endl;

	while(true)
	{
		if(selector.wait())
		{
			if(selector.isReady(listener))
			{
				std::shared_ptr<sf::TcpSocket> socket(new sf::TcpSocket);
				if (listener.accept(*socket) == sf::Socket::Done)
				{
					selector.add(*socket);
					sockets.emplace_back(socket);
				}
			}
			else
			{
				for(int i = 0; i < sockets.size(); i++)
				{
					if(selector.isReady(*sockets[i]))
					{
						sf::Packet packet;
						if(sockets[i]->receive(packet) == sf::Socket::Done)
						{
							for(int j = 0; j < sockets.size(); j++)
							{
								if(j == i) continue;
								sockets[j]->send(packet);
							}
						}
					}
				}
			}
		}
	}
}
