#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


struct Project;


int projectCount;
int availableA;
int availableB;
int availableC;
int availableD;
int availableE;
int sampleCount;

vector<Project> v_project ;

struct Project{

public:
  int a_ ;
  int b_ ;
  int c_ ;
  int d_ ;
  int e_ ;
  
  Project( int a,
  	   int b,
	   int c,
	   int d,
	   int e ) {
    a_ = a ;
    b_ = b ;
    c_ = c ;
    d_ = d ;
    e_ = e ;
  }
  
} ;
  
  
struct Player{

public:
  enum Target_id{ id_start_pos, id_samples, id_diagnosis, id_molecules, id_laboratory };

  int target_str_to_id( const string & target ) const {
    if ( !target.compare( "START_POS" ) ) {
      return id_start_pos ;
    }
    else if ( !target.compare( "SAMPLES" ) ) {
      return id_samples ;
    }
    else if ( !target.compare( "DIAGNOSIS" ) ) {
      return id_diagnosis ;
    }
    else if ( !target.compare( "MOLECULES" ) ) {
      return id_molecules ;
    }
    else if ( !target.compare( "LABORATORY" ) ) {
      return id_laboratory ;
    }
  }

  string target_id_to_str( const int & target_id ) const {
    switch( target_id )
      {
      case id_start_pos :
	return string( "START_POS" );
	break;
      case id_samples :
	return string( "SAMPLES" );
	break;
      case id_diagnosis :
	return string( "DIAGNOSIS" );
	break;
      case id_molecules :
	return string( "MOLECULES" );
	break;
      case id_laboratory :
	return string( "LABORATORY" );
	break;
      }
  }
  
  int target_;
  int eta_;
  int score_;
  int storageA_;
  int storageB_;
  int storageC_;
  int storageD_;
  int storageE_;
  int expertiseA_;
  int expertiseB_;
  int expertiseC_;
  int expertiseD_;
  int expertiseE_;


  Player( string target,
	  int eta,
	  int score,
	  int storageA,
	  int storageB,
	  int storageC,
	  int storageD,
	  int storageE,
	  int expertiseA,
	  int expertiseB,
	  int expertiseC,
	  int expertiseD,
	  int expertiseE ) {
    target_     = target_str_to_id( target ) ;
    eta_        = eta ;
    score_      = score ;
    storageA_   = storageA ;
    storageB_   = storageB ;
    storageC_   = storageC ;
    storageD_   = storageD ;
    storageE_   = storageE ;
    expertiseA_ = expertiseA ;
    expertiseB_ = expertiseB ;
    expertiseC_ = expertiseC ;
    expertiseD_ = expertiseD ;
    expertiseE_ = expertiseE ;
  } ;


  friend ostream &operator<<( ostream &output, 
			      const Player &p ) {
    output << "target : " << p.target_id_to_str( p.target_ ) << " "
	   << "eta: " << p.eta_ << " "
	   << "score: " << p.score_ << " "
	   << "storage: "
	   << p.storageA_ << " "
	   << p.storageB_ << " "
	   << p.storageC_ << " "
	   << p.storageD_ << " "
	   << p.storageE_ << " "
	   << "expertise: "
	   << p.expertiseA_ << " "
	   << p.expertiseB_ << " "
	   << p.expertiseC_ << " "
	   << p.expertiseD_ << " "
	   << p.expertiseE_ << " " ;
    return output;
  }


};


struct Sample{

public:
  int sampleId_ ;
  int carriedBy_ ;
  int rank_ ;
  string expertiseGain_ ;
  int health_ ;
  int costA_ ;
  int costB_ ;
  int costC_ ;
  int costD_ ;
  int costE_ ;

  Sample( int sampleId,
	  int carriedBy,
	  int rank,
	  string expertiseGain,
	  int health,
	  int costA,
	  int costB,
	  int costC,
	  int costD,
	  int costE )
  {
    sampleId_      = sampleId ;
    carriedBy_     = carriedBy ;
    rank_          = rank ;
    expertiseGain_ = expertiseGain ;
    health_        = health ;
    costA_         = costA ;
    costB_         = costB ;
    costC_         = costC ;
    costD_         = costD ;
    costE_         = costE ;
  } ;
  
  friend ostream &operator<<( ostream &output, 
			      const Sample &s ) { 
    output << "id : " << s.sampleId_ << " "
	   << "owner: " << s.carriedBy_ << " "
	   << "rank: " << s.rank_ << " "
	   << "gain: " << s.expertiseGain_ << " "
	   << "health: " << s.health_ << " "
	   << "cost: "
	   << s.costA_ << " "
	   << s.costB_ << " "
	   << s.costC_ << " "
	   << s.costD_ << " "
	   << s.costE_ << " ";
    return output;
  }

};


void read_input( vector<Player> & v_player,
		 vector<Sample> & v_sample ) {
  for (int i = 0; i < 2; i++) {
    string target;
    int eta;
    int score;
    int storageA;
    int storageB;
    int storageC;
    int storageD;
    int storageE;
    int expertiseA;
    int expertiseB;
    int expertiseC;
    int expertiseD;
    int expertiseE;

    cin >> target >> eta >> score
	>> storageA >> storageB >> storageC >> storageD >> storageE
	>> expertiseA >> expertiseB >> expertiseC >> expertiseD >> expertiseE;
    cin.ignore();

    Player player( target, eta, score,
		   storageA, storageB, storageC, storageD, storageE,
		   expertiseA, expertiseB, expertiseC, expertiseD, expertiseE ) ;
    
    v_player.push_back( player ) ;
  }

  cin >> availableA >> availableB >> availableC >> availableD >> availableE;
  cin.ignore();

  cin >> sampleCount;
  cin.ignore();
  for (int i = 0; i < sampleCount; i++) {
    int sampleId;
    int carriedBy;
    int rank;
    string expertiseGain;
    int health;
    int costA;
    int costB;
    int costC;
    int costD;
    int costE; 

    cin >> sampleId >> carriedBy >> rank >> expertiseGain >> health
	>> costA >> costB >> costC >> costD >> costE;
    cin.ignore();

    Sample sample( sampleId, carriedBy, rank, expertiseGain, health,
		   costA, costB, costC, costD, costE ) ;
    // cerr << sample << endl ;
    v_sample.push_back( sample ) ;
  }
}

/**
 * Bring data on patient samples from the diagnosis machine to the laboratory with enough molecules 
 * to produce medicine!
 **/
int main()
{
  cin >> projectCount;
  cin.ignore();

  for (int i = 0; i < projectCount; i++) {
    int a;
    int b;
    int c;
    int d;
    int e;
    cin >> a >> b >> c >> d >> e;
    cin.ignore();

    Project project( a, b, c, d, e ) ;
    v_project.push_back( project ) ;
  }

  // game loop
  while ( 1 ) {
    vector<Player> v_player ;
    vector<Sample> v_sample ;

    read_input( v_player, v_sample ) ;

    // cerr << v_sample << endl ;
    Player my_player = v_player[ 0 ];
    cerr << "my_player: " << my_player << endl ;
    cerr << "id_samples: " << my_player.id_samples << endl ;

    int alea ;
    alea = rand( ) ;
    cerr << "alea: " << alea << endl ;
    alea = alea  % 2 ;
    cerr << "alea: " << alea << endl ;
    if ( alea == 0 ) {
      cout << "GOTO DIAGNOSIS" << endl ;
    }
    else {
      cerr << "my player: " << my_player << endl ;
      cout << "CONNECT 1" << endl ;
    }
  }
}
