#include "server.h"
#include "httplib.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <numeric>
#include <filesystem>

std::unordered_map<int, std::shared_ptr<httplib::Server>> servers;
std::mutex mtx;

bool __stdcall create(const char* host_str, const char* directory_str, int port)
{
	destroy(port);
	{
		std::unique_lock<std::mutex> lck(mtx);
		std::shared_ptr<httplib::Server> svr = std::make_shared<httplib::Server>();
		std::string host = host_str;
		if (!host.length())
		{
			host = "127.0.0.1";
		}
		std::thread thread([svr, host, port]() {
			svr->listen(host, port);
			});
		thread.detach();
		svr->wait_until_ready();
		if (!svr->is_running()) {
			return false;
		}

		servers.emplace(port, svr);

		std::string directory = directory_str;
		if (directory.length())
		{
			svr->set_mount_point("/", directory);
		}

		svr->Post("file", [](const httplib::Request& req, httplib::Response& res) {
			if (!req.has_file("path"))
			{
				res.set_content("", "text/plain");
			}
			else {
				const auto& file_path = req.get_param_value("path");
				if (!std::filesystem::exists(file_path))
				{
					res.set_content("", "text/plain");
				}
				else
				{
					std::ifstream file(file_path, std::ios::binary);
					if (file.is_open())
					{
						if (!req.has_file("type") || req.get_param_value("type") != "binary")
						{
							std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
							res.set_content(content, "text/plain");
						}
						else
						{
							std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
							uint32_t* head = reinterpret_cast<uint32_t*>(content.data());
							size_t size = content.size() * sizeof(char) / sizeof(uint32_t);
							std::vector<std::string> hexes;
							for (size_t i = 0; i < size; i++)
							{
								std::stringstream ss;
								ss << "0x" << std::setfill('0') << std::hex << std::setw(2) << *(head + i);
								std::string hex = ss.str();
								hexes.push_back(hex);
							}
							std::string c = std::accumulate(
								std::next(hexes.begin()),
								hexes.end(),
								hexes[0],
								[](std::string a, std::string b) {
									return a + " " + b;
								}
							);
							res.set_content(c, "text/plain");
						}
						file.close();
					}
					else
					{
						res.set_content("", "text/plain");
					}
				}
			}
			});

		svr->set_error_handler([](const httplib::Request& req, httplib::Response& res) {
			const char* fmt =
				"<p>Error Status: <span style='color:red;'>%d</span></p>";
			char buf[BUFSIZ];
			snprintf(buf, sizeof(buf), fmt, res.status);
			res.set_content(buf, "text/html");
			});

		return true;
	}
}

void __stdcall destroy(int port)
{
	std::unique_lock<std::mutex> lck(mtx);
	for (const auto& server : servers)
	{
		if (server.first == port)
		{
			if (server.second->is_valid() && server.second->is_running())
			{
				server.second->stop();
			}
			servers.erase(server.first);
			break;
		}
	}
}
