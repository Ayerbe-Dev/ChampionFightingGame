#include "BattleObject.h"

/// <summary>
/// Load all entries for this object's param table.
/// </summary>
void BattleObject::load_params() {
	param_table.load_params(resource_dir + "/param/params.prmlst");
	param_table.load_params(resource_dir + "/param/stats.prmlst");
}