create table Manufacturer
(
	manufacturer_name varchar primary key
);

create table Part
(
	part_name varchar primary key, 
	manufacturer_name varchar references Manufacturer,
	mass number,
	cost number
);

create table Assembly 
(
	assembly_name varchar primary key
);

create table Assembly_Part
(
	part_name varchar references Part,
	assembly_name varchar references Assembly,
	quantity number,
	primary key(part_name, assembly_name)
);
