<?php

/*
|--------------------------------------------------------------------------
| Application Routes
|--------------------------------------------------------------------------
|
| Here is where you can register all of the routes for an application.
| It's a breeze. Simply tell Laravel the URIs it should respond to
| and give it the controller to call when that URI is requested.
|
*/

$app->get('/', function() {
    return view('index');
});

$app->put('temp/{data}', 'App\Http\Controllers\TempController@postTemp');
$app->get('temp/insert/{data}', 'App\Http\Controllers\TempController@postTemp');

$app->get('temp', 'App\Http\Controllers\TempController@getTemp');

$app->get('temp/since/{time}', 'App\Http\Controllers\TempController@getTempSince');

$app->get('temp/last', 'App\Http\Controllers\TempController@getLastTemp');

$app->delete('temp', 'App\Http\Controllers\TempController@truncate');

