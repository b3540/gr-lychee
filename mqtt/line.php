<?php
require( "./phpMQTT.php" );

$raw = file_get_contents('php://input');
$receive = json_decode($raw, true);
$accessToken = "";

$event = $receive['events'][0];
$messages = $receive['events'][0]["message"]["text"];
$reply_token  = $event['replyToken'];

$output_message = "";

$headers = array('Content-Type: application/json',
                 'Authorization: Bearer ' . $accessToken);

$mqtt_host = "mqtt server";
$mqtt_port = 1883; 
$mqtt_clientid = "clientid";
$mqtt_topic = "";

if ($messages == "cv") {
    $mqtt_topic = "opencv/";
    $output_message = "OpenCVの";
} else if ($messages == "image") {
    $mqtt_topic = "image/";
    $output_message = "RGBの";
}
$mqtt_message = '1';
$mqtt = new Bluerhinos\phpMQTT($mqtt_host, $mqtt_port, $mqtt_clientid );
if( $mqtt->connect() ){
  $mqtt->publish( $mqtt_topic, $mqtt_message, 0 );
  $mqtt->close();
}

$message = array('type' => 'text',
                 'text' => $output_message . "画像を取得します");

$body = json_encode(array('replyToken' => $reply_token,
                          'messages'   => array($message)));
$options = array(CURLOPT_URL            => 'https://api.line.me/v2/bot/message/reply',
                 CURLOPT_CUSTOMREQUEST  => 'POST',
                 CURLOPT_RETURNTRANSFER => true,
                 CURLOPT_HTTPHEADER     => $headers,
                 CURLOPT_POSTFIELDS     => $body);

$curl = curl_init();
curl_setopt_array($curl, $options);
curl_exec($curl);
curl_close($curl);

?>

