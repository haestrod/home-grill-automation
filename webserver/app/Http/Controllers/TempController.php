<?php namespace App\Http\Controllers;

use Laravel\Lumen\Routing\Controller as BaseController;
use App\Temp;
use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use DB;

class TempController extends BaseController
{
	public function postTemp($data) {
		$v = floatval($data); // voltage value
		// Temperature calculation
	        #$temp = 0.226584602 + 24152.109 * $v
		#	+ 67233.4248 * ($v**2)
        	#        + 2210340.682 * ($v**3)
		#	+ -860963914.9 * ($v**4)
                #	+ 48350600000 * ($v**5)
		#	+ -1.18452 * (10**12) * ($v**6)
                #	+ -1.3869 * (10**13) * ($v**7)
                #	+ -6.33708 * (10**13) * ($v**8);
		$temp = $v; // no temp calculation

		// Record the temperature
		$tempn = Temp::create(['temp' => $temp, 'voltage' => $v]);

		return response()->json('success');
	}

	public function getTemp() {
		$temps = Temp::where('createdAt', '>', time()-(60*60))->get();

		return response()->json($temps);
	}

	public function getTempSince($time) {
		$time = intval($time);

		if ($time >= time() ) {
			// Default to lat 60 seconds of data
			$data = Temp::where('createdAt', '>', time()-60)->get();
		} else {
			$data = Temp::where('createdAt', '>', $time)->get();
		}


		return response()->json($data);
	}

	public function getLastTemp() {
		$temp = Temp::orderBy('createdAt', 'desc')->first();
		return response()->json($temp);
	}

	public function truncate() {
		Temp::truncate();
		return response()->json('success');
	}
}

