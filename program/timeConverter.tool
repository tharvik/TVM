/* This program converts seconds the corresponding days/hours/minutes/seconds equivalent.
*/
/* Author : Thiébaud Modoux, 2013 */

object TimeConverter {
	
	def main(): Unit = {
		println(new Time().initialize(1282983278).getConvertion());
	}
}

class Time {
	
	var days: Int;
	var hours: Int;
	var minutes: Int;
	var seconds: Int;
	
	/* Initially, the only unit is seconds */
	def initialize(s: Int): Time = {
		seconds = s;
		return this;
	}
	
	/* Converts seconds in days, then converts the remaining part in hours/minutes/seconds */
	def convert(): Bool = {
		var finish: Bool;
		finish = false;
		if(0 < seconds || 0 == seconds) {
			days = seconds/(3600*24);
			seconds = seconds - days*3600*24;
			hours = seconds/3600;
			seconds = seconds - hours*3600;
			minutes = seconds/60;
			seconds = seconds - minutes*60;
			finish = true;
		}
		return finish;
	}
	
	/* Print the converted time */
	def getConvertion(): String = {
		var continue: Bool;
		var converted: String;
		
		continue = this.convert();
		
		if(continue) {
			converted = days + " days, " + hours + " hours, " + minutes + " minutes et " + seconds + " secondes";
		}
		else{
			converted = "Erreur";
		}
		return converted;
	}
}