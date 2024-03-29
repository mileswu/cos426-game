#include "world.h"
#include "ai.h"
#include "bubble.h"
#include "gl.h"
#include "shader.h"
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <SFML/Audio.hpp>
#include <stdio.h>

using namespace std;

double World::emission_speed = 5.0;
double World::emission_sizefactor = 0.05;
const double World::world_size = 25.0;

World::World() {
  // Initialize time.
  lasttime_updated.tv_sec = 0;
  lasttime_updated.tv_usec = 0;
  world_status = 0;
  level_of_detail = 1;
  trails_enabled = 1;
  powerups_enabled = 1;
  num_enemies = 0;
  num_bubbles = 200;
}

static double rand(double max, int plusminus = 0, double min = 0) {
  double retval = ((double)rand()) / RAND_MAX;
  retval *= max - min;
  retval += min;
  if (plusminus == 1 && rand() % 2 == 0) {
    retval *= -1;
  }
  return retval;
}

static R3Vector randvector(double max, double min = 0) {
  for (;;) {
    R3Vector retval(rand(max, 1), rand(max, 1), rand(max, 1));
    if (retval.Length() < max && retval.Length() > min) {
      return retval;
    }
  }
}

static R3Point randpoint(double max, double min = 0) {
  return randvector(max, min).Point();
}

Bubble *World::CreateBubble() {
  Bubble *b = new Bubble();
  b->pos = randpoint(20);
  b->v = randvector(0.2);
  b->size = rand(1.5, 0.1);
  b->player_id = -1;
  b->material = &Bubble::neutral_material;
  bubbles.push_back(b);
  return b;
}

Bubble *World::CreatePlayerBubble() {
  Bubble *player = new Bubble();
  player->size = 1.0;
  player->player_id = 0;
  player->material = &Bubble::player_material;
  bubbles.push_back(player);
  return player;
}

Bubble *World::CreateEnemyBubble() {
  Bubble *enemy = new Bubble();
  enemy->pos = randpoint(30);
  enemy->v = R3null_vector;
  enemy->size = 1.5;
  enemy->player_id = 1;
  enemy->material = &Bubble::enemy_material;

  EnemyAI *ai = new EnemyAI();
  ai->world = this;
  ai->self = enemy;
  ai->target = NULL;
  enemy->ai = ai;

  bubbles.push_back(enemy);
  return enemy;
}

void World::GenerateEmptyLevel() {
  // Generate player bubble.
  CreatePlayerBubble();
}

void World::GenerateRandomLevel() {
  // Generate player bubble.
  CreatePlayerBubble();

  // Generate dumb NPC bubbles.
  for (int i = 0; i < num_bubbles; i++) {
    CreateBubble();
  }

  // Generate enemy bubbles.
  for (int i = 0; i < num_enemies; i++) {
    CreateEnemyBubble();
  }

  // Generate random powerups.
  if(powerups_enabled == 1) {
    for (unsigned int i = 0; i < bubbles.size() / 10; i++) {
      int rand_num = floor(rand(5));
      PowerUpType type;
      switch (rand_num) {
      case 0:
        type = invincible_type;
        break;
      case 1:
        type = small_sink_type;
        break;
      case 2:
        type = sink_type;
        break;
      case 3:
        type = speed_up_type;
        break;
      case 4:
        type = slow_down_type;
        break;
      }
      CreatePowerUp(type);
    }
  }

}

void World::LoadMeshes() {
  invincible_mesh[0] = new R3Mesh();
  invincible_mesh[0]->Read("./models/mushroom.off");
  invincible_mesh[0]->Scale(1, 1, 1);
  invincible_mesh[1] = new R3Mesh();
  invincible_mesh[1]->Read("./models/lowmushroom.off");
  invincible_mesh[1]->Scale(1, 1, 1);
  
  smallsink_mesh[0] = new R3Mesh();
  smallsink_mesh[0]->Read("./models/pear.off");
  smallsink_mesh[0]->Scale(0.05, 0.05, 0.05);
  smallsink_mesh[1] = new R3Mesh();
  smallsink_mesh[1]->Read("./models/lowpear.off");
  smallsink_mesh[1]->Scale(0.05, 0.05, 0.05);
  
  sink_mesh[0] = new R3Mesh();
  sink_mesh[0]->Read("./models/octopus.off");
  sink_mesh[0]->Scale(0.05, 0.05, 0.05);
  sink_mesh[1] = new R3Mesh();
  sink_mesh[1]->Read("./models/lowoctopus.off");
  sink_mesh[1]->Scale(0.05, 0.05, 0.05);
  
  speedup_mesh[0] = new R3Mesh();
  speedup_mesh[0]->Read("./models/heart.off");
  speedup_mesh[0]->Scale(0.25, 0.25, 0.25);
  speedup_mesh[1] = new R3Mesh();
  speedup_mesh[1]->Read("./models/lowheart.off");
  speedup_mesh[1]->Scale(0.25, 0.25, 0.25);
  
  slowdown_mesh[0] = new R3Mesh();
  slowdown_mesh[0]->Read("./models/Sword01.off");
  slowdown_mesh[0]->Scale(0.001, 0.001, 0.001);
  slowdown_mesh[1] = new R3Mesh();
  slowdown_mesh[1]->Read("./models/lowSword01.off");
  slowdown_mesh[1]->Scale(0.001, 0.001, 0.001);
}

void World::CreatePowerUp(PowerUpType type) {
  PowerUpShape p;
  p.type = type;
  switch (type) {
  case invincible_type:
    p.mesh[0] = invincible_mesh[0];
    p.mesh[1] = invincible_mesh[0];
	p.die_time = glutGet(GLUT_ELAPSED_TIME) + 5000;
    break;
  case small_sink_type:
  p.mesh[0] = smallsink_mesh[0];
  p.mesh[1] = smallsink_mesh[1];
	p.die_time = glutGet(GLUT_ELAPSED_TIME) + 5000;
    break;
  case sink_type:
  p.mesh[0] = sink_mesh[0];
  p.mesh[1] = sink_mesh[1];
	p.die_time = glutGet(GLUT_ELAPSED_TIME) + 15000;
    break;
  case speed_up_type:
  p.mesh[0] = speedup_mesh[0];
  p.mesh[1] = speedup_mesh[1];
  p.die_time = glutGet(GLUT_ELAPSED_TIME) + 15000;
    break;
  case slow_down_type:
  p.mesh[0] = slowdown_mesh[0];
  p.mesh[1] = slowdown_mesh[1];
  p.die_time = glutGet(GLUT_ELAPSED_TIME) + 15000;
    break;
  }
  int rand_x = floor(rand(2));
  int rand_y = floor(rand(2));
  int rand_z = floor(rand(2));
  p.Center = R3Point(rand(20, rand_x, 0), rand(20, rand_y, 0), rand(20, rand_z, 0));
  
  power_ups.push_back(p);
}

void World::EmitAtBubble(Bubble *b, R3Vector direction) {
  // FIXME peter(5/14) AI bubbles need to emit at arbitrary position/velocity
  if (world_status != 0) {
    return;
  }
  direction.Normalize();

  double total_mass = b->Mass();
  R3Vector total_momentum = b->Mass() * b->v;
  R3Vector orig_v = b->v;

  Bubble *b_emitted = new Bubble();
  b_emitted->SetSizeFromMass(total_mass * emission_sizefactor);
  b->SetSizeFromMass(total_mass * (1 - emission_sizefactor));

  b_emitted->pos = b->pos - (b->size + b_emitted->size) * direction;
  b_emitted->v = orig_v - emission_speed * direction;
  b->v = (total_momentum - b_emitted->Mass() * b_emitted->v) / b->Mass();

  bubbles.push_back(b_emitted);
}

void World::Emit(R3Vector camera_direction) {
  //if(world_status != 0) return;
  //camera_direction.Normalize();
  Bubble *b = bubbles[0];
  EmitAtBubble(b, camera_direction);
/*
  double total_mass = b->Mass();
  R3Vector total_momentum = b->Mass() * b->v;
  R3Vector orig_v = b->v;
  
  Bubble *b_emitted = new Bubble();
  bubbles.push_back(b_emitted);
  b_emitted->SetSizeFromMass(total_mass*emission_sizefactor);
  b->SetSizeFromMass(total_mass*(1-emission_sizefactor));
  
  b_emitted->pos = b->pos - camera_direction*(b->size + b_emitted->size);
  b_emitted->v = orig_v - emission_speed*camera_direction;

  b->v = (total_momentum - b_emitted->Mass()*b_emitted->v)/b->Mass();
*/
}

void World::SetPlayerVelocity(R3Vector v) {
  bubbles[0]->v = v;
}

void World::SetPlayerPosition(R3Point p) {
  bubbles[0]->pos = p;
}

R3Point World::PlayerPosition() {
  return bubbles[0]->pos;
}

R3Vector World::PlayerDirection() {
  R3Vector direction = bubbles[0]->v;
  direction.Normalize();
  return direction;
}

double World::PlayerSize() {
  return bubbles[0]->size;
}

void World::RemovePowerUp(int index) {
  PowerUpShape temp = power_ups.back();
  power_ups[index] = temp;
  power_ups.pop_back();
}

string World::PlayerStatus() {
  stringstream ss;
  ss << "Player size: " << bubbles[0]->size << endl;
  ss << "Bubbles left (excluding yourself): " << bubbles.size() - 1 << endl;
  double max_size = 0;
  for (vector<Bubble *>::iterator it = bubbles.begin() + 1, ie = bubbles.end();
       it != ie; it++) {
    if ((*it)->size > max_size) {
      max_size = (*it)->size;
    }
  }
  ss << "Largest bubble size (excluding yourself): " << max_size << endl;
  return ss.str();
}

void World::Timestep() {
  struct timeval curtime;
  curr_time = 0;
  last_time = 0;
  timestep = 0;
  gettimeofday(&curtime, NULL);
  if (lasttime_updated.tv_sec != 0) {
    curr_time = curtime.tv_sec + 1.0e-6 * curtime.tv_usec;
    last_time = lasttime_updated.tv_sec + 1.0e-6 * lasttime_updated.tv_usec;
    timestep = curr_time - last_time;
  }
  lasttime_updated = curtime;
}

void World::SimulateParticles() {
  if (trails_enabled != 1) {
    return;
  }
  for (vector<Bubble *>::iterator it = bubbles.begin(), ie = bubbles.end();
        it != ie; ++it) {
    if (!(*it)->material->emits_particles) {
      continue;
    }
    if ((*it)->v.IsZero()) {
      continue;
    }

    double idealnumtogen = (*it)->material->particle_rate * timestep;
    int numtogen = 0;
    numtogen += idealnumtogen;

    if (rand(1.0) < idealnumtogen-numtogen) {
      numtogen++;
    }

    for (int j = 0; j < numtogen; ++j) {
      Particle *particle = new Particle();
      particle->parent = *it;

      R3Vector normal = -(*it)->v;
      normal.Normalize();
      R3Vector tangentplanevector = normal;
      tangentplanevector[2] += 1;
      tangentplanevector[0] += 1;
      tangentplanevector[1] += 1;
      tangentplanevector.Cross(normal);
      tangentplanevector.Normalize();
      double t1, t2;
      t1 = rand(2.0*M_PI);
      t2 = rand(1.0)*sin(M_PI/6.0);
      R3Vector direction = tangentplanevector;
      direction.Rotate(normal, t1);
      R3Vector vcrossn = direction;
      vcrossn.Cross(normal);
      direction.Rotate(vcrossn, acos(t2));
      direction.Normalize();
      particle->velocity = 1.5*direction;

      //printf("emit particle\n");
      particle->color[0] = (*it)->material->particle_color[0];
      particle->color[1] = (*it)->material->particle_color[1];
      particle->color[2] = (*it)->material->particle_color[2];
      particle->color[3] = (*it)->material->particle_color[3];
      particle->position = (*it)->pos;
      particle->lifetime = 2000. + glutGet(GLUT_ELAPSED_TIME);
      particle->is_point = true;
      particle->point_size = (*it)->material->particle_size;
      particles.push_back(particle);
    }
  }
}

void World::SimulateAI() {
  for (vector<Bubble *>::iterator it = bubbles.begin(), ie = bubbles.end();
       it != ie; ++it) {
    if (NULL == (*it)->ai) {
      continue;
    }

    // Make an action after a set delay.
    if (curr_time - (*it)->ai->last_action_time > (*it)->ai->delay) {
      (*it)->ai->ActFromState();
      (*it)->ai->last_action_time = curr_time;
    }
  }
}

void World::SimulatePowerups() {
  Bubble *player = bubbles[0];

  // Check if powerups mesh die
  for (unsigned int i = 0; i < power_ups.size(); i++) {
    double cur_time = glutGet(GLUT_ELAPSED_TIME);
    if (cur_time > power_ups[i].die_time) {
      RemovePowerUp(i);
      --i;
    }
  }

  // Check if powerup effect time already expired
  for (unsigned int i = 0; i < bubbles.size(); i++) {
    double cur_time = glutGet(GLUT_ELAPSED_TIME);
    if (cur_time > bubbles[i]->effect_end_time &&
        bubbles[i]->effect_end_time != -1) {
      bubbles[i]->state = reg_state;
      bubbles[i]->effect_end_time = -1;
    }
  }

  // Random chance for power ups to spawn
  if(powerups_enabled == 1) {
    double random_chance = rand(100);
    if (random_chance < 5) {
      int rand_num = floor(rand(5));
      PowerUpType type;
      switch (rand_num) {
      case 0:
        type = invincible_type;
        break;
      case 1:
        type = small_sink_type;
        break;
      case 2:
        type = sink_type;
        break;
      case 3:
        type = speed_up_type;
        break;
      case 4:
        type = slow_down_type;
        break;
      }
      CreatePowerUp(type);
    }
  }

  // Powerup get!
  for (unsigned int j = 0; j < power_ups.size(); j++) {
    if (!player->Collides(power_ups[j].mesh[0], power_ups[j].Center.Vector())) {
      continue;
    }
    PowerUpType type = power_ups[j].type;
    switch (type) {
    case invincible_type:
      player->state = invincible_state;
      player->effect_end_time = glutGet(GLUT_ELAPSED_TIME) + 5000;
      break;
    case small_sink_type:
      player->state = small_sink_state;
      player->effect_end_time = glutGet(GLUT_ELAPSED_TIME) + 2000;
      break;
    case sink_type:
      player->state = sink_state;
      player->effect_end_time = glutGet(GLUT_ELAPSED_TIME) + 5000;
      break;
    case speed_up_type:
      player->state = speed_up_state;
      player->effect_end_time = glutGet(GLUT_ELAPSED_TIME) + 1000;
      break;
    case slow_down_type:
      player->state = slow_down_state;
      player->effect_end_time = glutGet(GLUT_ELAPSED_TIME) + 1000;
      break;
    }
    RemovePowerUp(j);
    --j;
  }
}

void World::SimulateMotion(bool player_only = false) {
  Bubble* player = bubbles[0];
  double m1 = player->Mass();

  // A calculation
  for (vector<Bubble *>::iterator it = bubbles.begin();
       it < bubbles.end(); it++) {
    (*it)->a = R3Vector(0,0,0);
    if ((*it) != bubbles[0]) {
      if (player_only) {
        break;
      }
      // Player sink status moves NPC bubbles toward it.
      if (player->state == sink_state) {
        R3Vector towards = player->pos - (*it)->pos;
        double m2 = (*it)->Mass();
        double towards_mag = sqrt(towards.Dot(towards));
        (*it)->a += towards/towards_mag * m1 * m2/(towards_mag * towards_mag); 
      } else if (player->state == small_sink_state) {
        if ((*it)->size < player->size) {
          R3Vector towards = player->pos - (*it)->pos;
          double m2 = (*it)->Mass();
          double towards_mag = sqrt(towards.Dot(towards));
          (*it)->a += towards/towards_mag * m1 * m2/(towards_mag * towards_mag); 
        }
      }
    }
  }

  // V update
  for (vector<Bubble *>::iterator it = bubbles.begin();
       it < bubbles.end(); it++) {
    // Update bubble velocities.
    (*it)->v += (*it)->a*timestep;
    if ((*it) != bubbles[0] && player_only) {
      break;
    }
    if (player->state == speed_up_state && (*it) == player) {
	  (*it)->v *= 10;
      player->state = reg_state;
      player->effect_end_time = -1;
    } else if (player->state == slow_down_state && (*it) == player) {
      (*it)->v *= 1;
      player->state = reg_state;
      player->effect_end_time = -1;
    }
  }

  // P update
  for (vector<Bubble *>::iterator it = bubbles.begin();
       it < bubbles.end(); it++) {
    // Update bubbles.
    (*it)->pos += (*it)->v*timestep;
    if ((*it) != bubbles[0] && player_only) {
      break;
    }
  }

  for (vector<Particle *>::iterator it = particles.begin(); it < particles.end(); ++it) {
    // Update particles.
    (*it)->position += timestep * (*it)->velocity;
    (*it)->lifetime -= timestep;

    // Particle lifetime.
    if (glutGet(GLUT_ELAPSED_TIME) > (*it)->lifetime) {
      Particle *p = *it;
      it = particles.erase(it);
      delete p;
    }
  }
}

void World::SimulateCollisions() {
  // Bubble-bubble collision.
  for (vector<Bubble *>::iterator it = bubbles.begin();
       it < bubbles.end(); it++) {  
    for (vector<Bubble *>::iterator it2 = it+1;
         it2 < bubbles.end(); it2++) {      
      int retval = (*it)->Collides(*it2);
      if (retval == -1) {
        it = bubbles.erase(it);
        if (it==bubbles.begin()) {
          world_status = 1;
          PlayMusic(DEATH_SOUND);
        } else if (it2==bubbles.begin()) {
          PlayMusic(ABSORBING_SOUND);
        }
      } else if (retval == -2) {
        it2 = bubbles.erase(it2);
        if (it2==bubbles.begin()) {
          world_status = 1;
          PlayMusic(DEATH_SOUND);
        } else if (it==bubbles.begin()) {
          PlayMusic(ABSORBING_SOUND);
        }
      }
    }
  }

  // Check for collisions with the world border.
  R3Vector normal;
  for (vector<Bubble *>::iterator it = bubbles.begin();
       it < bubbles.end(); it++) {
    // Check to see if this position is too close to the wall
    // Note: world center is (0,0,0)
    if ((*it)->pos.Vector().Length() > (world_size - (*it)->size)) {
      normal = (*it)->pos.Vector();
      normal.Normalize();
      (*it)->pos = (normal * (world_size - (*it)->size-0.1)).Point();
      (*it)->v = 2.0 * ((*it)->v.Dot(normal)) * normal - (*it)->v;
      (*it)->v.Flip();
    }
  }
}

void World::Simulate() {
  // Based on bubble material, emit particle trail.
  SimulateParticles();

  // Perform AI action depending on the AI thinking rate.
  SimulateAI();

  // Create new powerups and expire old ones.
  SimulatePowerups();

  // A, V, P updates.
  SimulateMotion();

  // All collisions.
  SimulateCollisions();
}

void World::DrawOverlay() {
  if (world_status == 1) {
    glColor4f(0.0, 0.0, 0.0, 0.8);
    glBegin(GL_QUADS);
    glTexCoord2f (0.0, 0.0);
    glVertex3f (-1.0, -1.0, 0.0);
    glTexCoord2f (1.0, 0.0);
    glVertex3f (1.0, -1.0, 0.0);
    glTexCoord2f (1.0, 1.0);
    glVertex3f (1.0, 1.0, 0.0);
    glTexCoord2f (0.0, 1.0);
    glVertex3f (-1.0, 1.0, 0.0);
    glEnd();
    
    glColor4f(1.0, 1.0, 1.0, 1.0);
    
    glRasterPos2i(0.5,0.5);

    string osd_text = string("You died.");
    for (string::iterator it = osd_text.begin(); it < osd_text.end(); it++) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *it);
    }
  }
}

static double BubbleVectorIntersection(Bubble *b, R3Ray *ray) {
  R3Point c = b->pos;
  R3Vector l = c - ray->Start();
  double tca = l.Dot(ray->Vector());
  if (tca < 0.0) {
    return INFINITY;
  }
  double r2 = b->size * b->size;
  double d2 = l.Dot(l) - tca * tca;
  if (d2 > r2) {
    return INFINITY;
  }
  double thc = sqrt(r2 - d2);

  // The ray parameter t is the 'proper time' with velocity normalized to 1.
  double t = tca - thc;
  return t;
}

void World::Draw(R3Camera camera, Shader *bump_shader, int occlusion) {  
  glEnable(GL_LIGHTING);
  //int light_index = GL_LIGHT0 + 10;
  
  GLfloat c[4];
  double player_size = bubbles[0]->size;
  
  R3Ray player_ray(camera.eye, bubbles[0]->pos - camera.eye);
  double player_dist = BubbleVectorIntersection(bubbles[0], &player_ray);

  for (vector<Bubble *>::iterator it = bubbles.begin(), ie = bubbles.end();
       it != ie; it++) {
    bool transparency = false;

    if ((*it)->player_id == 0) {
      // The player is blue.
      c[0] = 0; c[1] = 0; c[2] = 1; c[3] = 1;
    } else if ((*it)->player_id == 1) {
      // The enemy is a sickly magenta color.
      c[0] = 1; c[1] = 0; c[2] = 1; c[3] = 1;
    } else {
      // The neutrals range from green to red depending on their size
      // relative to the player's.
      double s = (*it)->size;
      if (s < 0.8*player_size) {
        c[0] = 0; c[1] = 1; c[2] = 0; c[3] = 1;
      } else if (s > 1.2*player_size) {
        c[0] = 1; c[1] = 0; c[2] = 0; c[3] = 1;
      } else {
        double f = (s - 0.8*player_size)/(0.4*player_size);
        c[0] = f; c[1] = 1-f; c[2] = 0; c[3] = 1;
      }
    }

    // Prevent player occlusion by giving bubbles transparency.
    if (occlusion == 1 && (*it)->player_id != 0) {
      double t = BubbleVectorIntersection(*it, &player_ray);
      if (t < player_dist) {
        //printf("is transparent\n");
        transparency = true;
        //c[0] = c[1] = c[2] = 1.;
        c[3] = 0.4;
      }
    }

    /* Lighting idea
    if(0) {
    glDisable(light_index);
    glLightfv(light_index, GL_DIFFUSE, c);
    glLightfv(light_index, GL_SPECULAR, c);
    GLfloat gl_pos[4];
    gl_pos[0] = (*it)->pos[0]; gl_pos[1] = (*it)->pos[1]; gl_pos[2] = (*it)->pos[2]; gl_pos[3] = 0;
    glLightfv(light_index, GL_POSITION, gl_pos);
    glEnable(light_index);
    light_index++;
    }*/
	
	GLfloat c_new[4];
	GLfloat c_yellow[4] = {1, 1, 0, 1};
	//double limegreen_bot = 50 + 205 + 50;
	GLfloat c_limegreen[4] = {50/255.0, 205/255.0, 50/255.0, 1};
	GLfloat c_hotpink[4] = {1, 105/255.0, 180/255.0, 1};
    // Apply material.

	if ((*it) -> state != reg_state && (*it) == bubbles[0])
	{
	  double cur_time = glutGet(GLUT_ELAPSED_TIME);
      double factor = (cos(cur_time/10.0) + 1)/2.0;
	  BubbleState state = (*it) -> state;
	  switch (state) {
      case invincible_state:
        for (unsigned int k = 0; k < 3; k++) {
          c_new[k] = factor * c_yellow[k] + (1-factor) * c[k]; 
        }
        c_new[3] = 1;       
      break;
      case small_sink_state:
        for (unsigned int k = 0; k < 3; k++) {
          c_new[k] = factor * c_limegreen[k] + (1-factor) * c[k]; 
        }
        c_new[3] = 1;  
      break;
      case sink_state:
        for (unsigned int k = 0; k < 3; k++) {
          c_new[k] = factor * c_hotpink[k] + (1-factor) * c[k]; 
        }
        c_new[3] = 1; 
      break;
	  default:break;
	  }

	  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c_new);
	  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c_new);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c_new);
    }
	else {
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
    }

    GLfloat shininess = 75;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
    if (transparency) {
      if(bump_shader != NULL)
        glUseProgram(0);
      glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
    }
    if (InView(camera, (*it)->pos, (*it)->size)) {
      if( level_of_detail == 0 ||  ((*it)->pos - camera.eye).Length() < 20  )
        (*it)->Draw(0);
      else
        (*it)->Draw(1);
    }
    if (transparency) {
      if(bump_shader != NULL)
        glUseProgram(bump_shader->program);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
  }
}

class Sorter {
public:
  Sorter(R3Camera c) { camera = c; }
  R3Camera camera;
  bool operator()(Particle const *p1, Particle const *p2) const {
    double d1 = (p1->position - camera.eye).Length();
    double d2 = (p2->position - camera.eye).Length();
    return (d1 > d2);
  }
};

void World::DrawTrails(R3Camera camera) {
  if(trails_enabled == 0) return;
  if(particles.size() == 0) return;
  
  // Render particle trails. We want to disable lighting so that
  // the trail is always bright and lens-flare-ful.
  glDisable(GL_LIGHTING);
  sort(particles.begin(), particles.end(), Sorter(camera));
  
  GLfloat *colors = (GLfloat *)malloc(particles.size()*4*sizeof(float));
  GLfloat *positions = (GLfloat *)malloc(particles.size()*3*sizeof(float));
  
  double size = particles[0]->point_size;
  int count = 0;
  for (vector<Particle *>::iterator it = particles.begin(),
       ie = particles.end(); it != ie; ++it) {
    if ((*it)->is_point) {
      if (!InView(camera, (*it)->position, (*it)->point_size)) {
        continue;
      }
      
      GLfloat *c = &(colors[count*4]);
      c[0] = (*it)->color[0]; c[1] = (*it)->color[1]; c[2] = (*it)->color[2]; 
      c[3] = ((*it)->lifetime - glutGet(GLUT_ELAPSED_TIME))/2000.0;
      
      GLfloat *p = &(positions[count*3]);
      p[0] = (*it)->position[0]; p[1] = (*it)->position[1]; p[2] = (*it)->position[2];
      
      count++;
      
    } else {
      // FIXME peter custom textured particles?
    }
  }
  if(count != 0) {
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glPointSize(size);
    glVertexPointer(3, GL_FLOAT, 3*sizeof(float), positions);
    glColorPointer(4, GL_FLOAT, 4*sizeof(float), colors);
    glDrawArrays(GL_POINTS, 0, count);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
  }
  free(colors);
  free(positions);
}

void World::DrawPowerups(R3Camera camera) {
  glEnable(GL_LIGHTING);

  GLfloat c[4];
  GLfloat c_purple[4] = {0.5, 0, 0.5, 1};
  GLfloat c_yellow[4] = {1, 1, 0, 1};

  for (unsigned int i = 0; i < power_ups.size(); i++){
    R3Point center = power_ups[i].Center;
    double radius = power_ups[i].mesh[0]->Radius();
    if (InView(camera, center, radius)) {
      double cur_time = glutGet(GLUT_ELAPSED_TIME);
      double factor = (cos(cur_time/10.0) + 1)/2.0;
      for (unsigned int k = 0; k < 3; k++) {
        c[k] = factor * c_purple[k] + (1-factor) * c_yellow[k]; 
      }
      c[3] = 1;
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
      
      glPushMatrix();
      glTranslated(power_ups[i].Center[0], power_ups[i].Center[1], power_ups[i].Center[2]);
      
      // Level of Detail
      if(level_of_detail == 0 || (power_ups[i].Center - camera.eye).Length() < 30)
        power_ups[i].mesh[0]->Draw();
      else
        power_ups[i].mesh[1]->Draw();

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      power_ups[i].mesh[0]->bbox.Draw();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glPopMatrix();
    }
  }
  glDisable(GL_LIGHTING);
}

void World::DrawWorld(R3Camera camera) {
  glDisable(GL_LIGHTING);
  //glTranslated(bubbles[0]->pos[0], bubbles[0]->pos[1], bubbles[0]->pos[2]);
  glColor3d(1,1,1);
  R3Vector dist = bubbles[0]->pos - camera.eye;
  double size = world_size + 30;
  //double size = world_size * dist.Length();;
  static GLUquadricObj *glu_sphere = gluNewQuadric();
  gluQuadricTexture(glu_sphere, GL_TRUE);
  gluQuadricNormals(glu_sphere, (GLenum) GLU_SMOOTH);
  gluQuadricOrientation(glu_sphere, GLU_INSIDE);
  gluSphere(glu_sphere, size, 32, 32);
}

void DrawCircle(double x0, double y0, double size) {
  int nsteps = 16;
  glNormal3d(0, 0, -1);
  glBegin(GL_POLYGON);  
  for (int i = 0; i < nsteps; i++) {
    double angle = i * 2 * M_PI / nsteps;
    double x = size * cos(angle);
    double y = size * sin(angle);
    glVertex3d(x+x0, y+y0, 0);
  }
  glEnd();
}

void DrawTriangle(double x0, double y0, double size, int orientation=0) {
  //int nsteps = 16;
  glNormal3d(0, 0, -1);
  glBegin(GL_POLYGON);
  if (orientation == 1) {
    size *= -1;
  }
  glVertex3d(x0, y0+size, 0);
  glVertex3d(x0+fabs(size)/tan(M_PI/6.0), y0-size, 0);
  glVertex3d(x0-fabs(size)/tan(M_PI/6.0), y0-size, 0);
  glEnd();
}

void World::DrawMinimap() {  
  glDisable(GL_LIGHTING);  
  GLfloat c[4];
  double player_size = bubbles[0]->size;
  double player_x = bubbles[0] -> pos[0];
  double player_y = bubbles[0] -> pos[1];
  
  
  for (vector<Bubble *>::iterator it = bubbles.end()-1, ie = bubbles.begin();
       it >= ie; it--) {
    if ((*it)->size / player_size < 0.5) {
      continue;
    }

    if ((*it)->player_id == 0) {
      c[0] = 0; c[1] = 0; c[2] = 1; c[3] = 1;
    } else if ((*it)->player_id == 1) {
      c[0] = 1; c[1] = 0; c[2] = 1; c[3] = 1;
    } else {
      double s = (*it)->size;
      if(s < 0.8*player_size) {
        c[0] = 0; c[1] = 1; c[2] = 0; c[3] = 1;
      }
      else if(s > 1.2*player_size) {
        c[0] = 1; c[1] = 0; c[2] = 0; c[3] = 1;
      }
      else {
        double f = (s - 0.8*player_size)/(0.4*player_size);
        c[0] = f; c[1] = 1-f; c[2] = 0; c[3] = 1;
      }
    }
    c[0] = 0.5;
	glColor3f(0, 0, 0);
			
    double zdist = (*it)->pos[2] - bubbles[0]->pos[2];
    double absdist = ((*it)->pos - bubbles[0]->pos).Length();
    double size = 1.5/absdist;

    if ((*it)->player_id == 0 || size > 0.1) {
      size = 0.1;
    }
    //int orientation = 0;
    double scale = 10;
	double x_pos = (*it) -> pos[0]/scale;
	double x_diff = x_pos - player_x/scale;
	double y_pos = (*it) -> pos[1]/scale; 
	double y_diff = y_pos - player_y/scale;
	
	if (fabs(zdist) < 5) {
      DrawCircle(x_diff, y_diff, size);
    } else if (zdist < 0) {
      DrawTriangle(x_diff, y_diff, size + 0.03, 0);
    } else {
      DrawTriangle(x_diff, y_diff, size + 0.03, 1);
    }
	
	glColor3f(c[0], c[1], c[2]);
    if (fabs(zdist) < 5) {
      DrawCircle(x_diff, y_diff, size);
    } else if (zdist < 0) {
      DrawTriangle(x_diff, y_diff, size, 0);
    } else {
      DrawTriangle(x_diff, y_diff, size, 1);
    }
  }
}

bool World::InView(R3Camera camera, R3Point pos, double radius) {
  double dist;
  dist = R3SignedDistance(camera.right_plane, pos);
  if (dist > 0 && !(fabs(dist) < radius)) return false;
  dist = R3SignedDistance(camera.left_plane, pos);
  if (dist > 0 && !(fabs(dist) < radius)) return false;
  dist = R3SignedDistance(camera.top_plane, pos);
  if (dist > 0 && !(fabs(dist) < radius)) return false;
  dist = R3SignedDistance(camera.bottom_plane, pos);
  if (dist > 0 && !(fabs(dist) < radius)) return false;
  dist = R3SignedDistance(camera.camera_plane, pos);
  if (dist < 0 && !(fabs(dist) < radius)) return false;
  return true;
}

void World::PlayMusic(SoundType type) {
  static sf::Music music;
  string file;

  if (type == DEATH_SOUND) {
    file = "audio/kiss.wav";
  } else if (type == ABSORBING_SOUND) {
    file = "audio/crunch.wav";
  }

  // http://www.grsites.com/archive/sounds/category/23/?offset=0 
  if (!music.openFromFile(file)) {
    printf("failed to find music\n");
  }
  music.setVolume(75);
  music.play();
}

