

var addon = require('./build/Release/addon');
var express = require('express');
var morgan = require('morgan');
var path = require('path');
var app = express();

app.use(morgan('combined'));

app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname, '', 'index.html'));
});

app.get('/style.css', function (req, res) {
        res.sendFile(path.join(__dirname, '', 'style.css'));
        });

app.get('/main.js', function (req, res) {
        res.sendFile(path.join(__dirname, '', 'main.js'));
        });

app.get('/logo.jpg', function(req, res) {
        res.sendFile(path.join(__dirname, '', 'logo.jpg'));
        })

var answers = [];
app.get("/submit-name", function( req, res){
        
        
        var word=req.query.name;
        var number_of_mis=addon.dictionary_check(word);
        var final_answer="** "+word+" ** has "+number_of_mis+" misspellings";
        answers.push(final_answer);
        
        res.send(JSON.stringify(answers));// we are sending back imformation as a string using json
        
        });

var appends = [];
app.get("/submit-word", function( req, res){
        
        var word = req.query.word;
        var appended = addon.dictionary_add(word);
        var final_output = word + " : has been Added.";
        appends.push(final_output);
        
        res.send(JSON.stringify(appends));
        });

var port = 8080;
app.listen(port, function () {
           console.log(`App listening on port ${port}!`);
           });
