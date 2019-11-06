# Changelog

#### Starter-7.3 (06/11/2019):
    Fix some spelling errors.

#### Starter-7.2  (08/07/2019):
    Set the state in ALARM if at least one server is running twice.

#### Starter-7.1  (07/06/2019):
    Make it compatible with python process using package name.

#### Starter-7.0  (09/02/2018):
    Add a STANDBY state when a server is blocked more than MovingMaxDuration property.

#### Starter-6.12  (31/08/2017):
    Add a check for several instances for a server.

#### Starter-6.11  (03/05/2017):
	Increase spectrum attributes dim_x

#### Starter-6.10  (07/12/2016):
	Fix a problem when server is started with java -jar

#### Starter-6.9  (22/03/2016):
	Restore double fork to prevent defunct child processes

#### Starter-6.8  (18/01/2016):
	Delete dev added in dev_stop()

#### Starter-6.6  (30/06/2015):
	VC12 project added.

#### Starter-6.5  (30/06/2015):
	close stdout at server startup.

#### Starter-6.4  (13/01/2015):
	 Fix problem with very long server name (>50 chars) 

#### Starter-6.3  (09/11/2014):
	Add patches from Elettra.
	Improve level managemnt for startup.
	Replace tg->get_orb()->run() by tg->server_run() in main.

#### Starter-6.2  (01/10/2014):
	Regenerate code with Pogo-9 to be full compatible with Tango-9

#### Starter-6.1  (19/06/2014):
	Add a stop on DevStartAll when a DevStopAll is received.

#### Starter-6.0   (25/03/2014):
	Add OFF state when any server is running.
	Update file headers.

#### Starter-5.13  (09/01/2014):
	Fixe a case sensitive problem.

#### Starter-5.12  (13/06/2013):
	Tag following small changes for Windows compil and change in Makefile.am (Linux compil).

#### Starter-5.11  (05/04/2013):
	State polling problem fixed.

#### Starter-5.10  (20/03/2013):
	Check size of string spectums for Tango-8.1 compatibility.

#### Starter-5.9  (08/03/2013):
	Update file headers.

#### Starter-5_8  (06/03/2013):
	Regenerated with Pogo-8.2 and improve status.

#### Starter_5_7  (12/11/2012):
	Modify python process check (process could be pythonxxxx). 

#### Starter-5.6  (09/08/2012):
	Problem when no server to control fixed.

#### Starter-5.5  (29/05/2012):
	Improve vc9 and vc10 projects.

#### Starter-5.4  (25/05/2012):
	Remove warnings detected by cppcheck.

#### Starter-5.3  (31/03/2011):
	Use sigaction() instead sigset() to do not block SIGINT signal.

#### Starter-5.1  (23/02/2011):
	Tagging the Release_5_1 of the Starter project.

#### Starter-5.0  (11/02/2011):
	This commit was manufactured by cvs2svn to create tag.
