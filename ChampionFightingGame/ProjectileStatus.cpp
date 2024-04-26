#include "Projectile.h"
#include "Fighter.h"

void Projectile::status_none() {

}

void Projectile::enter_status_none() {

}

void Projectile::exit_status_none() {

}

void Projectile::status_move() {

}

void Projectile::enter_status_move() {

}

void Projectile::exit_status_move() {

}

void Projectile::load_projectile_status_scripts() {
	SET_STATUS_FUNC(PROJECTILE_STATUS_MOVE, &Projectile::status_move);
	SET_ENTRY_STATUS_FUNC(PROJECTILE_STATUS_MOVE, &Projectile::enter_status_move);
	SET_EXIT_STATUS_FUNC(PROJECTILE_STATUS_MOVE, &Projectile::exit_status_move);
}