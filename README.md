<body>
    <h1>Ft_IRC</h1>
    <p>ft_irc is a project where you create your own Internet Relay Chat (IRC) server. This is a remarkable opportunity to learn about network protocols, non-blocking I/O operations, and low-level programming in C++.</p>
    <h2>Table of Contents</h2>
    <ol>
        <li><a href="#about">About</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#usage">Usage</a></li>
    </ol>
    <h2 id="about">About</h2>
    <p>ft_irc is a project where you'll build an IRC server in C++. The server should support multiple clients, allow for authentication, setting a nickname, a username, joining a channel, and sending/receiving private messages.</p>
    <h2 id="installation">Installation</h2>
    <ol>
        <li>Clone the repository: <pre><code>git clone https://github.com/Referee95/0.fb.git</code></pre></li>
        <li>Compile the project: <pre><code>make</code></pre></li>
    </ol>
    <h2 id="usage">Usage</h2>
    <p>Launch your IRC server from the command line after running 'make'. You'll need to provide a port number and a password for client connections. Example:</p>
    <pre><code>./ircserv 6667 mypassword</code></pre>
    <p>⚠️ Note that this is only a server, you'll need a client to connect to it if you want to try the program properly.</p>
    <h3>Features:</h3>
    <ul>
        <li>Support multiple clients</li>
        <li>Authentication, set a nickname, a username</li>
        <li>Join a channel</li>
        <li>Send and receive private messages</li>
        <li>Distinguish between operators and regular users</li>
        <li>Implement specific commands for channel operators</li>
    </ul>
    
<h2>Subject</h2>

<p>For more information about the project, read the subject file.</p>
</body>
</html>

# 0.Facebook.com

🔆 Commands that we implemented:

| Command | Description |
|---------|-------------|
| Nick    | The NICK command is used to give the client a nickname or change the previous one. |
| User    | The USER command is used at the beginning of a connection to specify the username and realname of a new user. |
| Join    | The JOIN command indicates that the client wants to join the given channel(s), each channel using the given key for it. |
| Privmsg | The PRIVMSG command is used to send private messages between users, as well as to send messages to channels. |
| Kick    | The KICK command can be used to request the forced removal of a user from a channel. |
| Invite  | The INVITE command is used to invite a user to a channel. |
| Topic   | The TOPIC command is used to change or view the topic of the given channel. |
| Mode    | The MODE command is used to set or remove options (or modes) from a given target. Our user modes : i, o. Our channels modes: b,k,m,o,p,s,t,v |
| Kill    | The KILL command is used to close the connection between a given client and the server they are connected to. KILL is a privileged command and is available only to IRC Operators. |
| List    | The LIST command is used to get a list of channels along with some information about each channel. |
| Motd    | The MOTD command is used to get the “Message of the Day” of the given server. |
| Names   | The NAMES command is used to view the nicknames joined to a channel and their channel membership prefixes. |
| Oper    | The OPER command is used by a normal user to obtain IRC operator privileges. |
| Part    | The PART command removes the client from the given channel(s). |
| Pass    | The PASS command is used to set a ‘connection password’. If set, the password must be set before any attempt to register the connection is made. |
| Ping    | The PING command is sent by either clients or servers to check the other side of the connection is still connected and/or to check for connection latency, at the application layer. |
| Quit    | The QUIT command is used to terminate a client’s connection to the server. The server acknowledges this by replying with an ERROR message and closing the connection to the client. |
| Notice  | The NOTICE command is used to send notices between users, as well as to send notices to channels. The difference between NOTICE and PRIVMSG is that automatic replies must never be sent in response to a NOTICE message. |




