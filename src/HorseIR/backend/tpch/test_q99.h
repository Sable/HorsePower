

L simulateQ99(){
	// read all tables
	PROFILE(1, initTableByName((S)"lineitem"));
	PROFILE(2, initTableByName((S)"part"));
	PROFILE(3, initTableByName((S)"supplier"));
	PROFILE(4, initTableByName((S)"partsupp"));
	PROFILE(5, initTableByName((S)"orders"));
	PROFILE(6, initTableByName((S)"nation"));
	PROFILE(7, initTableByName((S)"region"));
	PROFILE(8, initTableByName((S)"customer"));
	R 0;
}

L testTPCHQ99(){
    P("** Start simulation for experiments\n");
    simulateQ99();
    P("** End\n");
    R 0;
}

