#include "BuildingGroups.h"
#include "BuildingGroup.h"
#include <ranges>

void V3::BuildingGroups::setInfrastructureCosts()
{
	for (const auto& buildingGroup: std::views::values(buildingGroups))
	{
		auto parentGroupName = buildingGroup->getParentName();
		while (!buildingGroup->getInfrastructureCost() && parentGroupName)
		{
			buildingGroup->setInfrastructureCost(tryGetInfraCost(parentGroupName));
			parentGroupName = tryGetParentName(parentGroupName);
		}
	}
}

void V3::BuildingGroups::setResourceCaps()
{
	for (const auto& buildingGroup: std::views::values(buildingGroups))
	{
		auto parentGroupName = buildingGroup->getParentName();
		while (!buildingGroup->possibleIsResourceCapped() && parentGroupName)
		{
			buildingGroup->setResourceCap(tryGetIsCapped(parentGroupName));
			parentGroupName = tryGetParentName(parentGroupName);
		}
	}
}

void V3::BuildingGroups::addBuildingGroup(const std::shared_ptr<BuildingGroup>& theBuildingGroup)
{
	buildingGroups[theBuildingGroup->getName()] = theBuildingGroup;
}

std::optional<std::string> V3::BuildingGroups::tryGetParentName(const std::optional<std::string>& theGroupName) const
{
	if (!theGroupName)
		return std::nullopt;

	const auto& possibleGroup = buildingGroups.find(theGroupName.value());
	if (possibleGroup != buildingGroups.end())
	{
		return possibleGroup->second->getParentName();
	}

	return std::nullopt;
}

std::optional<double> V3::BuildingGroups::tryGetInfraCost(const std::optional<std::string>& theGroupName) const
{
	if (!theGroupName)
		return std::nullopt;

	const auto& possibleGroup = buildingGroups.find(theGroupName.value());
	if (possibleGroup != buildingGroups.end())
	{
		return possibleGroup->second->getInfrastructureCost();
	}

	return std::nullopt;
}

std::optional<bool> V3::BuildingGroups::tryGetIsCapped(const std::optional<std::string>& theGroupName) const
{
	if (!theGroupName)
		return std::nullopt;

	auto possibleGroup = buildingGroups.find(theGroupName.value());
	if (possibleGroup != buildingGroups.end())
	{
		if (possibleGroup->second->possibleIsResourceCapped() && !*possibleGroup->second->possibleIsResourceCapped())
			return false;
		if (possibleGroup->second->possibleIsResourceCapped() && *possibleGroup->second->possibleIsResourceCapped())
			return true;
		while (possibleGroup->second->getParentName())
			possibleGroup = buildingGroups.find(*possibleGroup->second->getParentName());
		{
			if (possibleGroup->second->possibleIsResourceCapped() && !*possibleGroup->second->possibleIsResourceCapped())
				return false;
			if (possibleGroup->second->possibleIsResourceCapped() && *possibleGroup->second->possibleIsResourceCapped())
				return true;
		}
		return possibleGroup->second->possibleIsResourceCapped();
	}

	return std::nullopt;
}