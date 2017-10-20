
document.getElementById("defaultOpen").click();
var submit = document.getElementById("submit_btn");
submit.onclick = function() {
    //Make a request to the server and send the name
    var request =  new XMLHttpRequest();
    //Capture the response and store it in a variable
    
    
    request.onreadystatechange = function() {
        if(request.readyState == XMLHttpRequest.DONE)
        {
            //Take some action
            if(request.status==200)
            {
                var names = request.responseText;
                names=JSON.parse(names);
                var list = '';
                for(var i=0 ; i < names.length; i++)
                {
                    
                    list += '<li>' + names[i] + '</li>';
                }
                var ul=document.getElementById('answer');
                ul.innerHTML = list ;
            }
        }
        //Not done yet
    };
    //Make the request
    var nameInput=document.getElementById("text_field");
    var name = nameInput.value;
    request.open('GET','http://localhost:8080/submit-name?name='+name,true);
    request.send(null);
    //Capture a list of names and render it as a list
}

var add_submit = document.getElementById("word_submit");
add_submit.onclick = function() {
    var request = new XMLHttpRequest();
    
    request.onreadystatechange = function() {
        if(request.readyState == XMLHttpRequest.DONE)
        {
            //Take some action
            if(request.status==200)
            {
                var names = request.responseText;
                names=JSON.parse(names);
                var list = '';
                for(var i=0 ; i < names.length; i++)
                {
                    
                    list += '<li>' + names[i] + '</li>';
                }
                var ul=document.getElementById('append_list');
                ul.innerHTML = list ;
            }
        }
        //Not done yet
    };
    //Make the request
    var nameInput=document.getElementById("word_field");
    var name = nameInput.value;
    request.open('GET','http://localhost:8080/submit-word?word='+name+"\n",true);
    request.send(null);
    //Capture a list of names and render it as a list
    
}

function openTab(evt, tabName) {
    var i,tabcontent,tablinks;
    
    tabcontent = document.getElementsByClassName("tabcontent");
    for(i=0; i<tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }
    
    tablinks = document.getElementsByClassName("tablinks");
    for(i=0; i<tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
    }
    
    document.getElementById(tabName).style.display = "block";
    evt.currentTarget.className += " active";
}
