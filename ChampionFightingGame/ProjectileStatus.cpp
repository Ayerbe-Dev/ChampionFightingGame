#include "Projectile.h"
#include "Fighter.h"

void Projectile::status_none() {

}

void Projectile::enter_status_none() {

}

void Projectile::exit_status_none() {

}

void Projectile::load_projectile_status_scripts() {
	status_script[PROJECTILE_STATUS_NONE] = &Projectile::status_none;
	enter_status_script[PROJECTILE_STATUS_NONE] = &Projectile::enter_status_none;
	exit_status_script[PROJECTILE_STATUS_NONE] = &Projectile::exit_status_none;
}