
#include "Globals.h"


using namespace std;

cLevelData :: cLevelData( void )
{
	BasicSprites = NULL;
	BasicSpriteCount = 0;

	BG_red = 0;
	BG_green = 0;
	BG_blue = 0;

	Musicfile = "badmofo.mod";
}

cLevelData :: ~cLevelData( void )
{
	unsigned int i = 0;
	
	if( BasicSprites ) 
	{
		for( i = 0;i < BasicSpriteCount;i++ ) 
		{
			if( BasicSprites[i] ) 
			{
				delete BasicSprites[i];
				BasicSprites[i] = NULL; 
			} 
		}

		delete []BasicSprites;	
		BasicSprites = NULL;	
	}
	
	BasicSpriteCount = 0;	
}

void cLevelData :: AddSprite( cMVelSprite *Sprite )
{
	BasicSprites = (cMVelSprite**) realloc( BasicSprites, ++BasicSpriteCount * sizeof(cMVelSprite*) );
	BasicSprites[BasicSpriteCount - 1] = Sprite;
	return;
}

int cLevelData :: GetCollidingSpriteNum( SDL_Rect *Crect )
{
	if( BasicSprites && Crect ) 
	{
		for( unsigned int i = 0; i < BasicSpriteCount; i++ )
		{
			if( !BasicSprites[i] ) 
			{
				continue;
			}
            
			if( RectIntersect( &(const SDL_Rect&)BasicSprites[i]->GetRect( SDL_TRUE ), Crect ) )
			{
				return i;
			}
		}
	}

	return -1;
}

cBasicSprite *cLevelData :: GetCollidingSprite( SDL_Rect *Crect )
{
	if( BasicSprites && Crect ) 
	{
		for( unsigned int i = 0; i < BasicSpriteCount; i++ )
		{
			if( !BasicSprites[i] ) 
			{
				continue;
			}

			if( RectIntersect( &(const SDL_Rect&)BasicSprites[i]->GetRect( SDL_FALSE ), Crect ) )
			{
				return BasicSprites[i];
			}
		}
	}

	return NULL;
}

void cLevelData :: DeleteSprite( unsigned int number )
{
	// WHY???
	//if( (BasicSprites && number) < (EnemyCount && BasicSprites[number]) ) 
	//{
		cout<<"deleted "<<number;
		delete BasicSprites[number];
		BasicSprites[number] = NULL;
	//}
}

cLevel :: cLevel( void )
{
	levelfile = "none";
	pLevelData = NULL;
}

cLevel :: ~cLevel( void )
{
	Unload();
}

void cLevel :: Load( string filename )
{
	string full_filename = LEVEL_DIR + filename;

	if( !FileValid( full_filename ) )
	{
		printf( "Error : Level does not exist : %s\n", full_filename.c_str() );
		return;
	}

	Unload();
	
	DeleteAllParticleEmitter();
	DeleteAllBullets();
	DeleteAllEnemies();
	
	levelfile = filename;

	pLevelData = new cLevelData();

	ifstream ifs;
	ifs.open( full_filename.c_str(), ios :: in );

	char *contents = new char[500]; // maximal length of an line
	
	for( unsigned int i = 0; ifs.getline( contents, 500 ); i++ )
	{
		Parse( contents, i );
	}

	ifs.close();

	delete contents;
	
	pFramerate->Reset();
}

void cLevel :: Unload( void )
{
	if( pLevelData ) 
	{
		delete pLevelData;
		pLevelData = NULL;
	}

	levelfile.clear();
}

void cLevel :: Save( void )
{
	string filename = LEVEL_DIR;
	filename += levelfile.c_str();

	if( !FileValid( filename ) ) 
	{
		printf( "Error Level Save : Level file does not exist %s\n", filename.c_str() );
	}

	ofstream ofs( filename.c_str(), ios::out );

	if( !ofs )
	{
		printf( "Error Level Save: could not open the Level file %s\n", filename.c_str() );
		return;
	}

	unsigned int i;
	char row[300];

	sprintf( row,  "### Level Saved with MeatBall FX V.0 ###\n\n" );
	ofs.write( row, strlen( row ) );

	//sprintf( row,  "### Level Saved on the %s\n", Get_Curr_Time() );
	//ofs.write( row, strlen( row ) );

	sprintf( row, "Player %d %d\n", (int) pPlayer->Startposx, (int)window_height - (int)pPlayer->Startposy );
	ofs.write( row, strlen( row ) );

	sprintf( row,  "Music %s\n\n", pLevelData->Musicfile.c_str() );
	ofs.write( row, strlen( row ) );

	sprintf( row, "### Map Objects ###\n" );
	ofs.write( row, strlen( row ) );
	
	// Map Objects
	if( pLevel->pLevelData ) 
	{
		for( i = 0; pLevel->pLevelData->BasicSpriteCount > i; i++ )
		{
			if( !pLevel->pLevelData->BasicSprites[i] ) 
			{
				continue;
			}
			
			string image_filename = IMan->GetIdentifier( pLevel->pLevelData->BasicSprites[i]->srcimage );

			image_filename.erase( 0, strlen( PIXMAPS_DIR ) );
			
			if( pLevel->pLevelData->BasicSprites[i]->type == SPRITE_TYPE_MASSIVE ) 
			{
				sprintf(row,  "Sprite %s %d %d MASSIVE\n", image_filename.c_str(), (int) pLevel->pLevelData->BasicSprites[i]->posx, (int)window_height - (int)pLevel->pLevelData->BasicSprites[i]->posy );
			}
			else if( pLevel->pLevelData->BasicSprites[i]->type == SPRITE_TYPE_PASSIVE ) 
			{
				sprintf(row,  "Sprite %s %d %d PASSIVE\n", image_filename.c_str(), (int) pLevel->pLevelData->BasicSprites[i]->posx, (int)window_height - (int)pLevel->pLevelData->BasicSprites[i]->posy );
			}
			else if( pLevel->pLevelData->BasicSprites[i]->type == SPRITE_TYPE_HALFMASSIVE ) 
			{
				sprintf(row,  "Sprite %s %d %d HALFMASSIVE\n", image_filename.c_str(), (int) pLevel->pLevelData->BasicSprites[i]->posx, (int)window_height - (int)pLevel->pLevelData->BasicSprites[i]->posy );
			}
			else
			{
				printf( "Warning Level Saving : Unknow Map Object type : %d\n",pLevel->pLevelData->BasicSprites[i]->type );
				continue;
			}

			ofs.write( row, strlen( row ) );
		}
	}

	sprintf( row, "### Enemies ###\n" );
	ofs.write( row, strlen( row ) );
	
	// Enemies
	if( Enemies ) 
	{
		for( i = 0; EnemyCount > i; i++ )
		{
			if( !Enemies[i] )
			{
				continue;
			}

			if( Enemies[i]->Enemy_type == ENEMY_AF373 )
			{
				sprintf( row,  "Enemy %d %d %d\n", (int)Enemies[i]->Startposx, (int)Enemies[i]->Startposy, Enemies[i]->Enemy_type );
				ofs.write( row, strlen( row ) );
			}
			else
			{
				printf( "Level Saving : Wrong Enemy type : %d\n", Enemies[i]->Enemy_type );
			}
		}
	}

	printf( "Level saved to %s\n", levelfile.c_str() );
}

void cLevel :: Update( void )
{
	if( !Mix_PlayingMusic() )
	{
		string filename = MUSIC_DIR + pLevelData->Musicfile;
#define DEMO
#ifndef DEMO
		pAudio->PlayMusik( (char *)filename.c_str(),0,1 );
#endif
		
	}
}

void cLevel :: Draw( void )
{
	for( unsigned int i = 0; i < pLevelData->BasicSpriteCount;i++ )
	{
		if( !pLevelData->BasicSprites[i] ) 
		{
			continue;
		}

		pLevelData->BasicSprites[i]->Update();
		pLevelData->BasicSprites[i]->Draw( Renderer );
	}
}

void cLevel :: Parse( char* command, int line )
{
	if ( strlen( command ) <= 5 || *command == '#')
	{
		return;
	}
	
	char* str = command;
	int count = 0;
	int i = 0;
	
	str += strspn(str," ");
	
	while (*str)
	{
		count++;
		str += strcspn(str," ");
		str += strspn(str," ");
	}
	
	str = command; // reset
	
	char** parts = new char*[ count ];
	
	str += strspn(str," ");
	
	while(*str)
	{
		size_t len = strcspn( str," " );
		parts[i] = (char*)malloc( len + 1 );
		memcpy( parts[i], str, len );
		parts[i][len] = 0;
		str += len + strspn( str + len, " " );
        
        if (parts[i][len-1] == '\r')
            parts[i][len-1] = 0;
        
		i++;
        
        
	}
	
	// Level Message handler
	ParseLine( parts, count, line );
	
	for( i = 0; i < count ; i++ )
	{
		delete( parts[i] );
	}
	
	delete []parts;
}

int cLevel :: ParseLine( char ** parts, unsigned int count, unsigned int line )
{
	if ( strcmp( parts[0], "Sprite") == 0 )
	{
		if( count != 5 )
		{
			printf("%s : line %d Error : ", levelfile.c_str() , line );
			printf(" Sprite needs 5 parameters\n");
			return 0; // error
		}

		string full_filename = PIXMAPS_DIR;
		full_filename += parts[1];

		if( !FileValid( full_filename ) )
		{
			printf("%s : line %d Error : ", levelfile.c_str() , line );
			printf(" Sprite image does not exist\n");
			return 0; // error
		}

		if( !is_valid_number( parts[2] ) )
		{
			printf( "%s : line %d Error : ", levelfile.c_str() , line );
			printf( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}
		if( !is_valid_number( parts[3] ) )
		{
			printf("%s : line %d Error : ", levelfile.c_str() , line );
			printf( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}

		IMan->Add( LoadImage( full_filename.c_str(), colorkey ), full_filename );

		if( strcmp( parts[4], "MASSIVE" ) == 0 )
		{
			cMVelSprite *temp = new cMVelSprite( IMan->GetPointer( full_filename ), (double)atoi( parts[2] ), (double)window_height -(double)atoi( parts[3] ) );
			temp->type = SPRITE_TYPE_MASSIVE;
			pLevelData->AddSprite( temp );
		}
		else if( strcmp( parts[4], "PASSIVE" ) == 0 )
		{
			cMVelSprite *temp = new cMVelSprite( IMan->GetPointer( full_filename ), (double)atoi( parts[2] ), (double)window_height -(double)atoi( parts[3] ) );
			temp->type = SPRITE_TYPE_PASSIVE;
			pLevelData->AddSprite( temp );
		}
		else if( strcmp( parts[4], "HALFMASSIVE" ) == 0 )
		{
			cMVelSprite *temp = new cMVelSprite( IMan->GetPointer( full_filename ), (double)atoi( parts[2] ), (double)window_height - (double)atoi( parts[3] ) );
			temp->type = SPRITE_TYPE_HALFMASSIVE;
			pLevelData->AddSprite( temp );
		}
		else
		{
			printf("%s : line %d Error : ", levelfile.c_str() , line );
			printf( "%s is not a valid Sprite type\n", parts[4] );
			return 0; // error
		}
		
	}
	else if( strcmp( parts[0], "Enemy") == 0 )
	{
		if( count != 4 )
		{
			printf("%s : line %d Error : ", levelfile.c_str() , line );
			printf(" Enemy needs 4 parameters\n");
			return 0; // error
		}

		if( !is_valid_number( parts[1] ) )
		{
			printf("%s : line %d Error : ", levelfile.c_str() , line );
			printf( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}
		
		if( !is_valid_number( parts[2] ) )
		{
			printf("%s : line %d Error : ", levelfile.c_str() , line );
			printf( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}
		
		if( !is_valid_number( parts[3] ) )
		{
			printf("%s : line %d Error : ", levelfile.c_str() , line );
			printf( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}

		if( atoi( parts[3] ) == ENEMY_AF373 )
		{
			AddEnemy( atoi( parts[1] ), atoi( parts[2] ), atoi( parts[3] ) );
		}
		else
		{
			printf("%s : line %d Error : ", levelfile.c_str() , line );
			printf( "%s is not a valid Enemy type\n", parts[3] );
			return 0; // error
		}
	}
	else if( strcmp( parts[0], "Player") == 0 )
	{
		if( count != 3 )
		{
			printf("%s : line %d Error : ", levelfile.c_str() , line );
			printf(" Player needs 3 parameters\n");
			return 0; // error
		}
		if( !is_valid_number( parts[1] ) )
		{
			printf( "%s : line %d Error : ", levelfile.c_str() , line );
			printf( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}
		if( !is_valid_number( parts[2] ) )
		{
			printf("%s : line %d Error : ", levelfile.c_str() , line );
			printf( "%s is not a valid integer value\n", parts[1] );
			return 0; // error
		}

		pPlayer->SetPos( (double) atoi( parts[1] ), (double)window_height - (double) atoi( parts[2] ) );
		
		pPlayer->Startposx = (double) atoi( parts[1] );
		pPlayer->Startposy = (double)window_height - (double) atoi( parts[2] );
	}
	else if( strcmp( parts[0], "Music") == 0 )
	{
		string filename = MUSIC_DIR;
		filename += parts[1];

		if( !FileValid( filename ) ) 
		{
			printf( "%s : line %d Error : ", levelfile.c_str() , line );
			printf( "%s music file does not exist\n", parts[1] );
			return 0; // error			
		}

		pLevelData->Musicfile = parts[1];
	}
	else
	{
		printf("%s : line %d Error : ", levelfile.c_str() , line );
		printf(" Unknown definition : %s\n", parts[0] );
		return 0; // error
	}

	return 1; // Succesfull	
}
