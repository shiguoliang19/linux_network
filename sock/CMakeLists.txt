cmake_minimum_required(VERSION 3.8)

project(sock)

add_executable(sock_client client.c)

add_executable(sock_server server.c)