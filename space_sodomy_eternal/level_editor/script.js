var screen = document.getElementById("screen");
var ctx = screen.getContext("2d");
var blocks = [];
var turrets = [];
var exits = [];
var bonuses = [];

var sizeM = 25;
var size = 1;
for(var i=0; i<sizeM; i++){
	blocks.push([]);
	for(var j=0; j<sizeM; j++){
		blocks[i].push(0);
	}
}
function create(){
	sizeM = document.getElementById("size").value;
	blocks = [];
	for(var i=0; i<sizeM; i++){
		blocks.push([]);
		for(var j=0; j<sizeM; j++){
			blocks[i].push(0);
		}
	}
}

var scale = 1;
screen.height = screen.width = scale*size*sizeM;
var mouse = {x: 0, y: 0, down: 0, mode: 1};

function cell(v){
	return Math.floor(v/size);
}
function mouseMove(){
	mouse.x = event.pageX-screen.getBoundingClientRect().left;
	mouse.y = event.pageY-screen.getBoundingClientRect().top + document.body.getBoundingClientRect().top;
	if(mouse.down){
		if(mouse.mode < 2)
		blocks[Math.floor(mouse.x/size/scale)][Math.floor(mouse.y/size/scale)] = mouse.mode;
	}
	
}

function mouseDown(){
	mouse.down = 1;
	if(mouse.down){
		if(mouse.mode < 2)
		blocks[Math.floor(mouse.x/size/scale)][Math.floor(mouse.y/size/scale)] = mouse.mode;
	}
	if(mouse.mode == 2){
		turrets.push({x: Math.floor(mouse.x/size/scale)*size + size/2, y: Math.floor(mouse.y/size/scale)*size + size/2, 
		dir: document.getElementById("dir").value*Math.PI,
		w: document.getElementById("w").value*Math.PI,
		bv: document.getElementById("bv").value,
		cd: document.getElementById("cd").value
		});
		mouse.mode = 0;
	}
	if(mouse.mode == 3){
		exits.push({x: Math.floor(mouse.x/size/scale)*size + size/2, y: Math.floor(mouse.y/size/scale)*size + size/2, 
		});
		mouse.mode = 0;
	}
	if(mouse.mode == 4){
		bonuses.push({x: Math.floor(mouse.x/size/scale)*size + size/2, y: Math.floor(mouse.y/size/scale)*size + size/2, type: "shield"});
		mouse.mode = 0;
	}
}
function mouseUp(){
	mouse.down = 0;
}

function drawCircle(x, y, r, color){
	ctx.beginPath();
	ctx.arc(x*scale,y*scale, r*scale, 0, 2*Math.PI);
	ctx.fillStyle = color;
	ctx.fill();
}
function draw(){
	scale = document.getElementById("scale").value;
	screen.width = screen.height = sizeM*size*scale;

	ctx.clearRect(0,0,1000,1000);
	ctx.strokeStyle = "rgb(200, 200, 200)";
	ctx.fillStyle = "black";
	for(var x=0; x<sizeM; x++){
		for(var y=0; y<sizeM; y++){
			ctx.strokeRect(x*size*scale, y*size*scale, size*scale, size*scale);
			if(blocks[x][y]){
				ctx.fillRect(x*size*scale, y*size*scale, size*scale, size*scale);
			}
		}
	}
	for(var i=0; i<turrets.length; i++){
		drawCircle(turrets[i].x, turrets[i].y, size/2, "red");
		ctx.beginPath();
		ctx.moveTo(turrets[i].x*scale, turrets[i].y*scale);
		ctx.lineTo(turrets[i].x*scale + Math.cos(turrets[i].dir)*scale, turrets[i].y*scale+Math.sin(turrets[i].dir)*scale);
		ctx.strokeStyle = "black";
		ctx.stroke();
	}
	for(var i=0; i<exits.length; i++){
		drawCircle(exits[i].x, exits[i].y, size/2, "green");
	}
	for(var i=0; i<bonuses.length; i++){
		drawCircle(bonuses[i].x, bonuses[i].y, size/2, "blue");
	}
	if(mouse.mode==1)
	ctx.strokeStyle = "rgb(0, 200, 0)";
	else
	ctx.strokeStyle = "rgb(200, 0, 0)";
	ctx.strokeRect(Math.floor(mouse.x/size/scale)*size*scale, Math.floor(mouse.y/size/scale)*size*scale, size*scale, size*scale);
}

function getModel(s){
	var str = "" + sizeM + " " + sizeM + "\n";
	var sc = s/sizeM;
	for(var x=0; x<sizeM; x++){
		for(var y=0; y<sizeM; y++){
			if(blocks[y][x]){
				str += "1";
			}else{
				str += "0";
			}
			str += " ";
		}
		str += "\n";
	}
	str += "SPAWNPOINT 1 1";
	for(var i=0; i<turrets.length; i++){
		var t = turrets[i];
		str += " TURRET " 
		+ " POS " + (t.x-0.5) + " " + (t.y-0.5) + 
		" DIR " + t.dir + 
		" W " + t.w + 
		" CD " + t.cd + 
		" BV " + t.bv + 
		" END\n";
	}
	for(var i=0; i<exits.length; i++){
		var t = exits[i];
		str += " EXIT " + (t.x-0.5) + " " + (t.y-0.5) +  
		"\n";
	}
	for(var i=0; i<bonuses.length; i++){
		var t = bonuses[i];
		str += " BONUS POS " + (t.x-0.5) + " " + (t.y-0.5) +  " TYPE " + t.type + 
		" END\n";
	}
	
	return str;
}
function generate(){
	document.getElementById("output").innerHTML = getModel(1000);
}
setInterval(draw, 5);
















