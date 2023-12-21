#ifndef DEFINE_HPP
#define DEFINE_HPP

// WELCOME [001 - 002]
#define RPL_WELCOME(nick) ":HI_IRC 001 " + nick + " :Welcome to our irc!\n" \
                                                  "  / \\__\n"              \
                                                  " (    @\\____\n"         \
                                                  " /         O\n"          \
                                                  "/   (_____/\n"           \
                                                  "/_____/   U\n"

#define RPL_YOURHOST(nick) ":HI_IRC 002 " + nick + " :Your host is HI_IRC, running version 1.2\r\n"

// JOIN [353 - 366]
#define RPL_JOIN(nick, ip, channel) ":" + nick + "!" + nick + "@" + ip + " JOIN " + channel + "\r\n"
#define RPL_NAMREPLY(nick, channel, users) ":HI_IRC 353 " + nick + " = " + channel + " :" + users + "\r\n"
#define RPL_ENDOFNAMES(nick, channel) ":HI_IRC 366 " + nick + " " + channel + " :End of /NAMES list\r\n"

// MODE
#define RPL_MODE_PLUS_N(nick, channel) ":HI_IRC 324 " + nick + " " + channel + ": +n " + "\r\n"
#define RPL_MODE_MINUS_N(nick, channel) ":HI_IRC 324 " + nick + " " + channel + ": -n " + "\r\n"

#define RPL_MODE_T(nick, channel, mode) ":HI_IRC 324 " + nick + " " + channel + ": " + mode + "\r\n"

#define RPL_MODE_I(nick, ip, mode) ":" + nick + "!" + nick + "@" + ip + " MODE " + nick + ": " + mode + "\r\n"

#define RPL_MODE_K(nick, channel, mode) ":HI_IRC 324 " + nick + " " + channel + ": " + mode + "\r\n"
#define RPL_MODE_L(nick, channel, limit) ":HI_IRC 324 " + nick + " " + channel + ": +l " + limit + "\r\n"
#define ERR_BADCHANNELKEY(nick, channel) ":HI_IRC 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n"
// PART
// 채널에 전송
#define RPL_PART(nick, ip, channel) ":" + nick + "!" + nick + "@" + ip + " PART :" + channel + "\r\n"

// INVITE [341 - 345]
#define RPL_INVITING(nick, target, channel) ":HI_IRC 341 " + nick + " " + target + " " + channel + "\r\n"
#define RPL_INVITED(nick, channel) ":HI_IRC 345 " + nick + " :You have been invited to " + channel + "\r\n"

// KICK
#define RPL_KICK(nick, user, ip, channel, kickuser, msg) ":" + nick + "!" + user + "@" + ip + " KICK " + channel + " " + kickuser + " " + msg + "\r\n"

// NICK
#define RPL_NICK(nick, user, ip, newnick) ":" + nick + "!" + user + "@" + ip + " NICK :" + newnick + "\r\n"

// LIST [321 - 323]
#define RPL_LISTSTART(nick) ":HI_IRC 321 " + nick + " Channel : UsersNum\r\n"
#define RPL_LIST(nick, channel, numusers) ":HI_IRC 322 " + nick + " " + channel + " " + numusers + "\r\n"
#define RPL_LISTEND(nick) ":HI_IRC 323 " + nick + " :End of /LIST\r\n"

// QUIT
#define RPL_QUIT(nick, user, ip, msg) ":" + nick + "!" + user + "@" + ip + " QUIT " + msg + "\r\n"

// PONG
#define RPL_PONG(ip) ":HI_IRC PONG " + ip + "\r\n"

// OP/DEOP
#define RPL_OP(nick, user, ip, channel, opuser) ":" + nick + "!" + user + "@" + ip + " MODE " + channel + " +o " + opuser + "\r\n";
#define RPL_DEOP(nick, user, ip, channel, deopuser) ":" + nick + "!" + user + "@" + ip + " MODE " + channel + " -o " + deopuser + "\r\n";

// PRIVMSG
#define RPL_PRIVMSG(nick, user, ip, receive, msg) ":" + nick + "!" + user + "@" + ip + " PRIVMSG " + receive + " " + msg + "\r\n"

// NOTICE
#define RPL_NOTICE(nick, user, ip, receive, msg) ":" + nick + "!" + user + "@" + ip + " NOTICE " + receive + " " + msg + "\r\n"

// TOPIC

#define RPL_NOTOPIC(nick, channel) ":PPL_IRC 331 " + nick + " " + channel + " :No topic is set\r\n"                                                             // topic 설정 안했을 때
#define RPL_TOPIC(nick, channel, topic) ":PPL_IRC 332 " + nick + " " + channel + " :" + topic + "\r\n"                                                          // topic 설정 했을 때
#define RPL_SET_TOPIC(nick, userName, hostName, channel, topic) ":" + nick + "!" + userName + "@" + hostName + " " + "TOPIC " + channel + " :" + topic + "\r\n" // topic 설정

// ERR_MSG [401 - 506]
#define ERR_NOSUCHNICK(nick, target) ":HI_IRC 401 " + nick + " " + target + " :No such nick\r\n"
#define ERR_NOSUCHSERVER(serv) ":HI_IRC 402 " + serv + " :No such server\r\n"
#define ERR_NOSUCHCHANNEL(nick, channel) ":HI_IRC 403 " + nick + " " + channel + " :No such channel\r\n"
#define ERR_CANNOTSENDTOCHAN(nick, channel) ":HI_IRC 404 " + nick + " " + channel + " :Cannot send to channel\r\n"
#define ERR_NOORIGIN ":HI_IRC 409 * :No origin specified\r\n"
#define ERR_NORECIPIENT(nick, command) ":HI_IRC 411 " + nick + " :No recipient given " + command + "\r\n"
#define ERR_NOTEXTTOSEND(nick) ":HI_IRC 412 " + nick + " :No text to send\r\n"
#define ERR_UNKNOWNCOMMAND ":HI_IRC 421 * :Command not found\r\n"

#define ERR_NONICKNAMEGIVEN ":HI_IRC 431 :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick) ":HI_IRC 432 " + nick + " :" + nick + " is Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(nick) ":PPL_IRC 433 " + nick + " " + " :Nickname is already in use\r\n"
#define ERR_NICKCOLLISION(nick) ":HI_IRC 436 " + nick + " :Nickname collision KILL\r\n"
#define ERR_USERNOTINCHANNEL(nick, target, channel) ":HI_IRC 441 " + nick + " " + target + " " + channel + " :They aren't on that channel\r\n"
#define ERR_NOTONCHANNEL(nick, channel) ":HI_IRC 442 " + nick + " " + channel + " :You're not on that channel\r\n"
#define ERR_NOTREGISTERED ":HI_IRC 451 * :You have not registered\r\n"

#define ERR_NEEDMOREPARAMS(nick, command) ":HI_IRC 461 " + nick + " " + command + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTRED(nick) ":HI_IRC 462 " + nick + " :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH ":HI_IRC 464 FAIL :Password incorrect\r\n"

#define ERR_CHANNELISFULL(nick, channel) ":HI_IRC 471 " + nick + " " + channel + " :Cannot join channel, it's full\r\n"
#define ERR_UNKNOWNMODE(nick, flags) ":HI_IRC 472 " + nick + " " + flags + " :is unknown mode char to me\r\n"
#define ERR_INVITEONLYCHAN(nick, channel) ":HI_IRC 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n"
#define ERR_NOPRIVILEGES(nick) ":HI_IRC 481 " + nick + " :Permission Denied- You're not an IRC operator\r\n"
#define ERR_CHANOPRIVSNEEDED(nick, channel) ":HI_IRC 482 " + nick + " " + channel + " :You're not channel operator\r\n"
#define ERR_NOOPERHOST(nick) ":HI_IRC 491 " + nick + " :No O-lines for your host\r\n"
#define ERR_UMODEUNKNOWNFLAG(nick, channel) ":HI_IRC 501 " + nick + " " + channel + " :Unknown MODE flag\r\n"
#define ERR_USERSDONTMATCH(nick) ":HI_IRC 502 " + nick + " :Cant change mode for other users\r\n"
#define ERR_FLOOD(sec) ":HI_IRC 504 * :flood detected, please wait " + sec + " seconds\r\n"

#define ERR_NOKEY(nick, channelName) ":PPL_IRC 696 " + nick + " " + channelName + " k * :You must specify a parameter for the key mode. Syntax: <key>.\r\n"

#endif