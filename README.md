# IRC <a name="top"></a>

### _The first messenger of the world in your hands_

Internet Relay Chat or IRC is a text-based communication protocol on the Internet.  
It offers real-time messaging that can be either public or private. Users can exchange direct messages and join group channels.  
IRC clients connect to IRC servers in order to join channels.

<img src="https://github.com/etorren1/IRC/blob/main/images/vid1.gif" width="70%"/>

## Compile and run MacOS and Linux

`make && ./ircserv port password`  

<img src="https://github.com/etorren1/IRC/blob/main/images/img1.png" width="70%"/>

## Allowed commands

| Connection Registration | Channel operations | Server queries and commands | Sending messages | User based queries | Miscellaneous messages| Optionals |
|-------------------------|--------------------|-----------------------------|------------------|--------------------|-----------------------|-----------|
|PASS                     |JOIN                |VERSION                      |PRIVMSG           |WHO                 |KILL                   |AWAY       |
|NICK                     |PART                |TIME                         |NOTICE            |WHOIS               |PING                   |REHASH     |
|USER                     |MODE                |ADMIN                        |                  |WHOWAS              |PONG                   |RESTART    |
|OPER                     |TOPIC               |INFO                         |                  |                    |                       |USERHOST   |
|QUIT                     |NAMES               |                             |                  |                    |                       |ISON       |
|                         |LIST                |                             |                  |                    |                       |           |
|                         |INVITE              |                             |                  |                    |                       |           |
|                         |KICK                |                             |                  |                    |                       |           |

## IRC bot 

To simplify work from the terminal, the project has a handbook bot with IRC command list


#### Compile and run MacOS and Linux

`cd Bot; make && ./ircbot host port password; cd..`  

<img src="https://github.com/etorren1/IRC/blob/main/images/img3.png" width="70%"/>

<img src="https://github.com/etorren1/IRC/blob/main/images/img4.png" width="70%"/>

## Client

You can also connect from a client writen by the RFC-1459 standard  
We tested at the Adium(Version 1.5.10.4)

<img src="https://github.com/etorren1/IRC/blob/main/images/img2.png" width="70%"/>

## Developers
[etorren](https://github.com/etorren1)  
[fhyman](https://github.com/askhabchic)  
[kmeeseek](https://github.com/lesorres)
***
<a href="#top">Gotop</a>
