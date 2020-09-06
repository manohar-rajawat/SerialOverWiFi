const char rootPage[] PROGMEM = R"===(
<!DOCTYPE html>
<html>
    <head>
        <title>Real Time Serial Data</title>
    </head>
    <body>
      <style>
          *, *:before, *:after {
            -moz-box-sizing: border-box;
            -webkit-box-sizing: border-box;
            box-sizing: border-box;
          }
          html {
            font-family: Helvetica, Arial, sans-serif;
            font-size: 100%;
            background: #333;
          }
          #page-wrapper {
            border-radius: 15px;
            width : 100%;
            height : 100%;
            background: #FFF;
            padding: 1em;
            margin: 1em auto;
            border-top: 5px solid #69c773;
            box-shadow: 0 2px 10px rgba(0,0,0,0.8);
          }
          
          #header {
                  display: inline-block;
          }
          
          #clear {
                  float: right;
          }
          
          h1 {
                  margin-top: 0;
          }
          #status {
            font-size: 0.9rem;
            margin-bottom: 1rem;
          }
          .open {
            color: green;
          }
          .closed {
            color: red;
          }
          
          ul:empty {
            display: none;
          }
          
          #messages {
           padding: 10px 10%;
          }
          
          ul {
            overflow: auto;
            height: 300px;
            list-style: none;
            margin: 0;
            padding: 0;
            font-size: 0.95rem;
            border: 4px solid #AADF93;
            border-radius: 10px;
            background-color: #8599a0;
            font-variant: petite-caps;
          }
          ul li {
            padding: 0.5rem 0.75rem;
            #border-bottom: 1px solid #EEE;
          }
          ul li:first-child {
            border-top: 1px solid #EEE;
          }
          ul li span {
            display: inline-block;
            width: 90px;
            font-weight: bold;
            color: #999;
            font-size: 0.7rem;
            text-transform: uppercase;
            letter-spacing: 1px;
          }
          .sent {
            #background-color: #F7F7F7;
            margin-top: 8px;
            margin-left: 2px;
            background-color: white;
            border: 2px solid white;
            width: max-content;
            border-radius: 32px;
          }
          .received {
           margin-top: 8px;
           margin-left: auto;
           color: white;
           width: max-content;
           border-radius: 32px;
           border: 2px solid white;
          }
          #message-form {
            margin-top: 1.5rem;
          }
          textarea {
            width: 100%;
            padding: 0.5rem;
            font-size: 1rem;
            border: 1px solid #D9D9D9;
            border-radius: 3px;
            box-shadow: inset 0 1px 1px rgba(0, 0, 0, 0.1);
            min-height: 100px;
            margin-bottom: 1rem;
          }
          button {
            display: inline-block;
            border-radius: 3px;
            border: none;
            font-size: 0.9rem;
            padding: 0.6rem 1em;
            color: white;
            margin: 0 0.25rem;
            text-align: center;
            background: #BABABA;
            border-bottom: 1px solid #999;
          }
          button[type="submit"] {
            background: #86b32d;
            border-bottom: 1px solid #5d7d1f;
          }
          button:hover {
            opacity: 0.75;
            cursor: pointer;
          }
          .buttons-float{
            float: right;
          }
          #close{
            background: indianred;
          }
      </style>
      <div id="page-wrapper">
        <div id="top-page">
          <h1 id="header">Real Time Serial Data</h1>
          <div class="buttons-float">
            <button type="submit" id="subscribe">Subscribe</button>
            <button type="button" id="close">Un-Subscribe</button>
            <button type="button" id="clear">Clear Data</button>
          </div>
        </div>
      
        <div id="status">Connecting...</div>
        <ul id="messages"></ul>
      </div>
    
      <script>
        var caller = on_Load();
        window.onload = caller;
        
        function on_Load()
        {
        var globalId=0;
        var status_S = document.getElementById('status');
        var messages_S = document.getElementById('messages');
        var close_S = document.getElementById('close');
        var form_S = document.getElementById('subscribe');
        var clear_S = document.getElementById('clear');
        
        var socket = new WebSocket("ws://"+window.location.host+":81");
        
        socket.onopen = function(event)
        {
         console.log('The connection has been initialized with the serve');
         status_S.innerHTML = 'The Connection is ready';
         status_S.className = 'open';
        }
        
        socket.onerror = function(error)
        {
         console.log('The error occurred during connection : '+error);
        }
        
        socket.onmessage = function(event)
        {
         globalId += 1;
         const tempId = 'temp'+globalId;
         //console.log('The message from the server is : '+event.data);
         const message = event.data;
         messages_S.innerHTML += '<li class="received" id="'+tempId+'"'+'>'+message+'</li>';
         //typingEffect(message,tempId);
         const thisMessage = document.getElementById(tempId);
         messages_S.scrollTop = thisMessage.offsetHeight + thisMessage.offsetTop
         
        }
        
        form_S.onclick = function(e)
        {
         e.preventDefault();
         socket.send('#');
         messages_S.innerHTML += '<li class="sent">Subscribed</li>';
         return false;
        }
     
        socket.onclose = function(event)
        {
         status_S.innerHTML = 'The connection is in closed state !!!';
         status_S.className = 'closed';
        }
        
        close_S.onclick = function(e)
        {
         e.preventDefault();
         socket.send('$');
         messages_S.innerHTML += '<li class="sent">Un-Subscribed</li>';
         return false;
        }
        
        clear_S.onclick = function(e)
        {
         e.preventDefault();
         messages_S.innerHTML = '';
         //messages_S.style.display='none';
         return false;
        }
          
        function typingEffect(message,tempId)
          {
            var index_v = 0;
            var size = message.length;
            var topic = document.getElementById(tempId);
            typing(index_v,size,topic,message);
          }
          
          function typing(index_v,size,topic,message)
          {
            if(index_v<size)
              {
                topic.innerHTML += message[index_v];
                index_v++;
                setTimeout(typing,20,index_v,size,topic,message);
              }
            else 
              return false;
          }
        
        }
      </script>
    </body>
</html>
)===";
