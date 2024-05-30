#include "system.h"

String getHtmlStyle()
{
  String  page ="<style>";
  page +="body {margin-top:50px; font-family:Arial; text-align:center;color: black;}";
  page +="table{width:100%;}";  
  page +=".btn{width:1100px;margin:20px auto; padding:30px; background: #48D1CC;";
  page +="font-size:60px; color:black; text-decoration:none;vertical-align:middle;}";
  page +="input, button{padding: 20px; font-size:80px; margin:25px 0; width:700px;text-align:center;}";
  page +="select{font-size:80px; margin: 20px 0; width:700px;text-align:center;padding:20px;}";  
  page +="select.channel{width:400px;}";  
  page +="h1,h2{margin:50px auto; text-align:center;}";
  page +="h1{font-size:80px;}";
  page +="h2{font-size:60px;}";
  page +="a{font-size:60px; /*margin:50px auto;*/ text-align:center;}";  
  page +="a{ height:200px;display:flex;justify-content: center;align-items: center;}";
  page +="table.root a{ width:700px;}";
  page +="table.settings a{ width:320px;}";
  page +="table.time-settings input, table.time-settings select{ font-size:40px;width:200px;}";
  page +="table.time-settings td{ font-size:40px;}";
  page +="span {font-size:80px; margin-right: 20px}";
  page +="tr.gray {vertical-align:middle;}";
  page +="tr.gray, table.time-settings tr:nth-child(odd){background:#EEE;}";
  page +="button {background: #87CEFA;border:0px solid white;padding:40px;}";  
  page +="input[type='submit']{background: #FF4500;border:0px solid white;padding:40px;}";
  page += "div.outputs{width:100%;display:flex;justify-content: space-around;align-items: center;flex-direction:row;margin:100px 0;}";
  page += "div.outputs div{display:flex;width:160px; height:160px;margin:0;padding:0;justify-content: center;align-items: center;font-size:60px;}";
  page += ".auto-on{ border-radius:80px; background: red; }";
  page += ".auto-off{ border-radius:100px; background: #DDD; }";
  page += ".manual-on{ background: red; width:80px; height:80px; }";
  page += ".manual-off{ background: #DDD; width:80px; height:80px; }";
  page += ".display{background:green;color:white;}";

  page += ".container {display:flex;flex-direction:column;justify-content:center;align-items:center;}";
  page += ".item {width: 100%;}";
  page += ".settings-btn{height:200px;width:700px;margin:20px auto;background:#48D1CC;font-size:60px;color:black;text-decoration:none;vertical-align:middle;}";

  page +="</style>";
  return page;
}
