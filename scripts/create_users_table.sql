-- drop database users;
create database if not exists users;
use users;
create table if not exists users (username VARCHAR(255) PRIMARY KEY, password VARCHAR(255));
create table if not exists messages (password VARCHAR(255), login_to VARCHAR(255), text VARCHAR(8191));
