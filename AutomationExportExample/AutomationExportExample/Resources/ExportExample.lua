--D:\workspace\AutomationCarExport\AutomationExportExample\AutomationExportExample\Resources\ExportExample.lua
---------------------------------------------------
-- Copyright (c) 2021 -- Camshaft Software PTY LTD
---------------------------------------------------

function DoExport(CarCalculator, CarFile)
	UAPI.Log("DoExport: ")

	local value = {}

	local EngineFiles = CExporter.ExportEngineFile(CarCalculator)
	local CarData = CExporter.ExportCarData(CarCalculator)
	local CarFiles = CExporter.ExportCarFiles(CarCalculator)
	CarFiles[string.format("%s - %s.car", CarCalculator.CarInfo.PlatformInfo.Name, CarCalculator.CarInfo.TrimInfo.Name)] = CarFile

	local Data = {}

	for k,v in pairs(CarData) do
		Data[k] = v
	end

	local Files = {}

	for k,v in pairs(EngineFiles) do
		Files[k] = v
	end

	for k,v in pairs(CarFiles) do
		Files[k] = v
	end

	return Files, Data
end



if CExporter == nil then
	CExporter = {}
	CExporter.__index = CExporter
end

--File Defs

local RPMTorquePair = "\t\t\t\t[%i, %.2f],\n"

local EngineDefinition =
[[
{

"torques":[$TorqueTable$]

			"idleRPM":$IdleRPM$,
			"maxRPM":$MaxRPM$, //Max RPM before overrev damage is likely

"gearbox":$Gearbox$,
]]


local Gearbox =
[[
	"gearRatios":[$GearRatios$],
]]



function CExporter.ExportEngineFile(CarCalculator)
	local CarInfo = CarCalculator.CarInfo
	local EngineInfo = CarInfo.TrimInfo.EngineInfo
	local GearboxInfo = CarInfo.TrimInfo.Gearbox
	local Results = EngineInfo.ModelInfo.Results

	local OutputText = ""

	local TorqueTable = ""

	local TorqueCurves = EngineInfo.ModelInfo.Results.UE4Curves
	

	for k,torque in ipairs(TorqueCurves.Torque) do
		TorqueTable = TorqueTable .. string.format(RPMTorquePair, TorqueCurves.RPM[k], torque)
	end



	OutputText = string.gsub(OutputText, "%$TorqueTable%$", TorqueTable)
	OutputText = string.gsub(OutputText, "%$IdleRPM%$", tostring(IdleRPM))
	OutputText = string.gsub(OutputText, "%$MaxRPM%$", tostring(MaxRPM))


	local GearboxRatios, DiffRatio = CarCalculator:GetRealGearRatios()

	local gearRatioText = ""

	gearRatioText = gearRatioText .. string.format("-%.2f, 0, ", GearboxRatios[1] * 1.05)

	for gearNum, ratio in pairs(GearboxRatios) do
		gearRatioText = gearRatioText .. string.format("%.2f", ratio)

		if gearNum < #GearboxRatios then
			gearRatioText = gearRatioText .. ", "
		end
	end


	local gearbox = Gearbox


	gearbox = string.gsub(gearbox, "%$GearRatios%$", gearRatioText)
	OutputText = string.gsub(OutputText, "%$Gearbox%$", gearbox)
	
	local carName = string.format("%s %s", CarCalculator.CarInfo.PlatformInfo.Name,
											CarCalculator.CarInfo.TrimInfo.Name)

	

	return { ["Camso_Engine.txt"] = OutputText,  } 
end

function CExporter.ExportCarData(CarCalculator)
	local CarInfo = CarCalculator.CarInfo
	local EngineInfo = CarInfo.TrimInfo.EngineInfo
	local GearboxInfo = CarInfo.TrimInfo.Gearbox
	local Results = CarInfo.TrimInfo.Results
	local SuspensionDetails = CarInfo.TrimInfo.SuspensionDetails

	local carParameters = CarCalculator:GetCarParameters()

	local Data = {}

	Data.MaxRPM = EngineInfo.ModelInfo.Results.MaxRPM
	Data.GameVersion = UAPI.GetGameVersion()
	--CarCalculator:CacheWeights()
	--CarCalculator:CalculateCachedSuspensionDetails()

	Data.TotalWeight = Results.Weight

	local CachedWeights = CarInfo.TrimInfo.CachedWeights

	Data.GearboxWeight = CachedWeights.gearboxWeight
	Data.EngineWeight = EngineInfo.ModelInfo.Results.Weight
	Data.EngineWeight = Data.EngineWeight + CachedWeights.PowerSteering
	Data.FrontDriveWeight = CachedWeights.frontDriveWeight
	Data.RearDriveWeight = CachedWeights.rearDriveWeight

	Data.EngineUID = EngineInfo.ModelInfo.UID

	Data.EnginePlacement = CarInfo.PlatformInfo.EnginePlacement.Placement
	Data.EngineOrientation = CarInfo.PlatformInfo.EnginePlacement.Orientation

	local DiscWeightToUnsprung = 1

	Data.FrontRim = CachedWeights.frontRim + CachedWeights.frontDisc * (2.0 - DiscWeightToUnsprung)
	Data.RearRim = CachedWeights.rearRim + CachedWeights.rearDisc * (2.0 - DiscWeightToUnsprung)
	Data.FrontTyre = CachedWeights.frontTyre
	Data.RearTyre = CachedWeights.rearTyre

	Data.FrontWheel = Data.FrontRim + Data.FrontTyre
	Data.RearWheel =  Data.RearRim + Data.RearTyre

	Data.FrontUnsprung = CachedWeights.frontUnsprung - CachedWeights.frontWheel * 2 - CachedWeights.frontDisc * DiscWeightToUnsprung + CachedWeights.ChassisWeight * 0.15
	Data.RearUnsprung = CachedWeights.rearUnsprung - CachedWeights.rearWheel * 2 - CachedWeights.rearDisc * DiscWeightToUnsprung + CachedWeights.ChassisWeight * 0.15

	Data.FrontSubframe = CachedWeights.ChassisWeight * 0.35 + CachedWeights.bottomWeight * 0.15
	Data.RearSubframe = CachedWeights.ChassisWeight * 0.35 + CachedWeights.bottomWeight * 0.15

	Data.SprungWeight = CachedWeights.sprung

	Data.BottomWeight = CachedWeights.bottomWeight * 0.7
	Data.TopWeight = CachedWeights.topWeight

	Data.InteriorWeight = CachedWeights.interiorWeight + CachedWeights.Entertainment + CachedWeights.Safety
	
	local BodyBoxes = CarCalculator:GetBodyBoxes()

	Data.TopBoxY = BodyBoxes.Top.Pos[1]
	Data.TopBoxLength = BodyBoxes.Top.Size[1]

	Data.BodyWeight = Data.BottomWeight + Data.TopWeight + Data.InteriorWeight

	Data.ChassisWeight = CachedWeights.ChassisWeight - CachedWeights.ChassisWeight * 0.6 + Data.FrontDriveWeight + Data.RearDriveWeight
	Data.WheelBase = CarInfo.TrimInfo.Body.WheelBase

	Data.Stiffness = (CarCalculator:CalculateStiffness() / 42000) ^ 0.5

	Data.WeightDistribution = CarCalculator:CalculateDynamicCG(0).WeightDistribution

	CExporter.ExportWheelData(CarCalculator, Data)

	local frontBrakes, rearBrakes = CarCalculator:GetBrakingForces()

	Data.FrontBrakeTorque = frontBrakes.BrakingTorque * 0.95
	Data.RearBrakeTorque = rearBrakes.BrakingTorque * 0.95
	Data.EBrakeTorque = Data.RearBrakeTorque * 2.0
	
	if CarCalculator.AccelerationDetails and CarCalculator.AccelerationDetails.Braking and CarCalculator.AccelerationDetails.Braking[10] then
		local rearTyreForce = math.abs(CarCalculator.AccelerationDetails.Braking[10].F_xr_st)
		rearTyreForce = rearTyreForce * 2
		
		local TrimInfo = CarInfo.TrimInfo
		local TyreDetails = TrimInfo.TyreDetails
		
		Data.EBrakeTorque = rearTyreForce * (TyreDetails.Front.OverallDiameter * 0.001)
		
	end
	
	local AeroMode = "ACCELERATION"
	Data.FrontalArea = CarCalculator:GetTotalEffectiveArea(AeroMode)

	local brakes = CarInfo.TrimInfo.Brakes

	Data.FrontBrakeDiameter = brakes.Front.Diameter * 0.001 * 1.1
	Data.RearBrakeDiameter = brakes.Rear.Diameter * 0.001 * 1.1

	Data.FrontBrakeType = "disc"
	Data.RearBrakeType = "disc"

	Data.FrontDiscMass = CachedWeights.frontDisc
	Data.RearDiscMass = CachedWeights.rearDisc

	Data.FrontbrakeVentingCoef = 1.0
	Data.RearbrakeVentingCoef = 1.0

	Data.FrontPadMaterial = "basic"
	Data.RearPadMaterial = "basic"

	if brakes.Front.Discs.Name == "Brake_Drum_Name" or brakes.Front.Discs.Name == "Brake_TwinLeadingDrum_Name" then Data.FrontBrakeType = "drum" end
	if brakes.Rear.Discs.Name == "Brake_Drum_Name" or brakes.Rear.Discs.Name == "Brake_TwinLeadingDrum_Name" then Data.RearBrakeType = "drum" end

	Data.FrontBrakeMaterial = "steel"
	Data.RearBrakeMaterial = "steel"

	if brakes.Front.Discs.Name == "BrakeDisc_CarCeramic_Name" then
		Data.FrontBrakeMaterial = "ceramic"
		Data.FrontBrakeType = "vented-disc"
	end

	if brakes.Rear.Discs.Name == "BrakeDisc_CarCeramic_Name"then
		Data.RearBrakeMaterial = "ceramic"
		Data.RearBrakeType = "vented-disc"
	end

	if brakes.Front.Discs.Name == "BrakeDisc_Vented_Name" then Data.FrontBrakeType = "vented-disc" end
	if brakes.Rear.Discs.Name == "BrakeDisc_Vented_Name"then Data.RearBrakeType = "vented-disc" end

	if brakes.Rear.Discs.Name == "BrakeDisc_Solid_Name" then Data.FrontbrakeVentingCoef = 1.0 end
	if brakes.Rear.Discs.Name == "BrakeDisc_Vented_Name" then Data.FrontbrakeVentingCoef = 1.1 end
	if brakes.Rear.Discs.Name == "BrakeDisc_CarCeramic_Name" then Data.FrontbrakeVentingCoef = 1.2 end

	if brakes.Rear.Discs.Name == "BrakeDisc_Solid_Name" then Data.RearbrakeVentingCoef = 1.0 end
	if brakes.Rear.Discs.Name == "BrakeDisc_Vented_Name" then Data.RearbrakeVentingCoef = 1.1 end
	if brakes.Rear.Discs.Name == "BrakeDisc_CarCeramic_Name" then Data.RearbrakeVentingCoef = 1.2 end
	
	
	Data.FrontbrakeVentingCoef = Data.FrontbrakeVentingCoef + CarInfo.TrimInfo.BrakeCoolingFraction
	Data.RearbrakeVentingCoef = Data.RearbrakeVentingCoef + CarInfo.TrimInfo.BrakeCoolingFraction

	if brakes.Front.PadType > 0.8 then
		Data.FrontPadMaterial = "full-race"
		Data.RearPadMaterial = "full-race"
	elseif brakes.Front.PadType > 0.6 then
		Data.FrontPadMaterial = "semi-race"
		Data.RearPadMaterial = "semi-race"
	elseif brakes.Front.PadType > 0.4 then
		Data.FrontPadMaterial = "sport"
		Data.RearPadMaterial = "sport"
	elseif brakes.Front.PadType > 0.2 then
		Data.FrontPadMaterial = "premium"
		Data.RearPadMaterial = "premium"
	end

	Data.FrontTravel = CarInfo.TrimInfo.CachedSuspension.CaseA.TravelRemainingFront * 0.001 * 1.1
	Data.RearTravel = CarInfo.TrimInfo.CachedSuspension.CaseA.TravelRemainingRear * 0.001 * 1.1

	Data.FrontBumpStopProgression = CarInfo.TrimInfo.CachedSuspension.CaseA.TravelRemainingFront * 0.001 * 0.1
	Data.RearBumpStopProgression = CarInfo.TrimInfo.CachedSuspension.CaseA.TravelRemainingRear * 0.001 * 0.1

	local FrontSpringMulti = 1.2
	local RearSpringMulti = 1.2

	local FrontDamperMulti = 1.44
	local RearDamperMulti = 1.44

	local FrontARBStiffnessMulti = 1000
	local RearARBStiffnessMulti = 1000

	local FrontPrecompressionMulti = 25
	local RearPrecompressionMulti = 25

	if CarCalculator.CarInfo.PlatformInfo.FrontSuspension.Name == "Suspend_SolAxCoil_Name" or
			CarCalculator.CarInfo.PlatformInfo.FrontSuspension.Name == "Suspend_SolAxLeaf_Name"
	then
		--FrontARBStiffnessMulti = 500
		FrontPrecompressionMulti = 15
	end

	if CarCalculator.CarInfo.PlatformInfo.RearSuspension.Name == "Suspend_SolAxCoil_Name" or
			CarCalculator.CarInfo.PlatformInfo.RearSuspension.Name == "Suspend_SolAxLeaf_Name"
	then
		--RearARBStiffnessMulti = 500
		RearPrecompressionMulti = 15
	end

	Data.FrontSpringStiffness = SuspensionDetails.Front.SpringStiffness * FrontSpringMulti
	Data.RearSpringStiffness = SuspensionDetails.Rear.SpringStiffness * RearSpringMulti

	Data.FrontDamperStiffness = SuspensionDetails.Front.DamperStiffness * FrontDamperMulti
	Data.RearDamperStiffness = SuspensionDetails.Rear.DamperStiffness * RearDamperMulti

	Data.FrontDamperFast = Data.FrontDamperStiffness / 3
	Data.FrontDamperReboundFast = Data.FrontDamperStiffness / 2

	Data.RearDamperFast = Data.RearDamperStiffness / 3
	Data.RearDamperReboundFast = Data.RearDamperStiffness / 2

	Data.FrontSwayBarStiffness = math.max(100, SuspensionDetails.Front.ARBStiffness) * FrontARBStiffnessMulti * (carParameters.FrontTrackWidth / 1.20)
	Data.RearSwayBarStiffness = math.max(100, SuspensionDetails.Rear.ARBStiffness) * RearARBStiffnessMulti * (carParameters.RearTrackWidth / 1.20)

	--Precompression is the old way this was done
	Data.FrontSpringPrecompression = 1 + (CarCalculator.CarInfo.TrimInfo.CachedSuspension.CaseA.FrontWheelWeight / Data.FrontSpringStiffness) * FrontPrecompressionMulti
	Data.RearSpringPrecompression = 1 + (CarCalculator.CarInfo.TrimInfo.CachedSuspension.CaseA.RearWheelWeight / Data.RearSpringStiffness) * RearPrecompressionMulti
	
	Data.FrontSpringPrecompressionRange = (CarCalculator.CarInfo.TrimInfo.CachedSuspension.CaseA.FrontWheelWeight * 9.81)  / Data.FrontSpringStiffness
	Data.RearSpringPrecompressionRange =  (CarCalculator.CarInfo.TrimInfo.CachedSuspension.CaseA.RearWheelWeight * 9.81) / Data.RearSpringStiffness

	Data.FrontCamber = SuspensionDetails.Front.Camber
	Data.RearCamber = SuspensionDetails.Rear.Camber

	--Aero
	Data.FrontWingAngle = math.lerp(0, 18, CarInfo.TrimInfo.InclinationFront)
	Data.RearWingAngle =  math.lerp(0, 18, CarInfo.TrimInfo.InclinationRear)

	--Need to do engine size and position
	local results = { CarCalculator:CheckEngineSize() }

	local bounds = results[5]

	if bounds then
		Data.EngineMinX = bounds.Min.x
		Data.EngineMinY = -bounds.Max.y
		Data.EngineMinZ = bounds.Min.z

		Data.EngineMaxX = bounds.Max.x
		Data.EngineMaxY = -bounds.Min.y
		Data.EngineMaxZ = bounds.Max.z
	end

	local GearboxInfo = CarInfo.TrimInfo.Gearbox

	--Corey can add specific Data/Tunes for drive types here
	if GearboxInfo.DriveType.DrivenWheels == "FRONT" then
        --in degrees here, converted to precompression in jbeam
        --affects dblwishbone, strut, semitrailing only
        Data.RearToe = -0.05
	elseif GearboxInfo.DriveType.DrivenWheels == "REAR" then
        Data.RearToe = 0.35
	else
        Data.RearToe = 0.25
	end

	return Data
end

function CExporter.ExportWheelData(CarCalculator, Data)

	local CarInfo = CarCalculator.CarInfo
	local TrimInfo = CarInfo.TrimInfo
	local Results = CarInfo.TrimInfo.Results
	local SuspensionDetails = CarInfo.TrimInfo.SuspensionDetails
	local FrontWheelParameters = CarCalculator:GetFrontTyreParameters()
	local RearWheelParameters = CarCalculator:GetRearTyreParameters()
	local TyreDetails = TrimInfo.TyreDetails


	local SuspensionTypeWeights =
	{
		["Suspend_MacPher_Name"] = { Front = 113.5 - 48.0, Rear = 98.5 - 48.0 },
		["Suspend_DblWishbone_Name"] = { Front = 147.5 - 80.0, Rear = 123.5 - 80.0 },
		["Suspend_SolAxCoil_Name"] = { Front = 130.0, Rear = 110.0 },
		["Suspend_SolAxLeaf_Name"] = { Front = 130.0, Rear = 110.0 },
		["Suspend_Multilink_Name"] = { Front = 147.5 - 80.0, Rear = 123.5 - 80.0 },
		["Suspend_SemiTrailArm_Name"] = { 					 Rear = 45 },
		["Suspend_Pushrod_Name"] = { Front = 147.5 - 80.0, Rear = 123.5 - 80.0 },
		["Suspend_Torsion_Name"] = { 						 Rear = 54 },
	}

	--We need some other data for tyres
	Data.TyreGripFront = CarCalculator:GetFrontTyreParameters().u_xf * 1.26
	Data.TyreGripRear = CarCalculator:GetRearTyreParameters().u_xr * 1.26
	Data.FullLoadCoefFront = CarCalculator:GetFrontTyreParameters().u_xf * 0.5
	Data.FullLoadCoefRear = CarCalculator:GetRearTyreParameters().u_xr * 0.5

	Data.TreadCoeff = 0.5
	Data.SlidingFrictionCoef = 1.0
	Data.TireStiffnessCoef = 1

	if	CarCalculator.CarInfo.TrimInfo.Tyres.Name == "TyreType_SemiOffRoad_Name" then
		Data.TreadCoeff = 1
		Data.SoftnessCoef = 0
		Data.TireStiffnessCoef = 0.3
		Data.LoadSenseCoef = 0.7
	elseif CarCalculator.CarInfo.TrimInfo.Tyres.Name == "TyreType_LongHardRoad_Name" then
		Data.TreadCoeff = 0.8
		Data.SoftnessCoef = 0.2
		Data.TireStiffnessCoef = 0.7
		Data.LoadSenseCoef = 0.7
	elseif CarCalculator.CarInfo.TrimInfo.Tyres.Name == "TyreType_MedComp_Name" then
		Data.TreadCoeff = 0.5
		Data.SoftnessCoef = 0.4
		Data.TireStiffnessCoef = 0.8
		Data.LoadSenseCoef = 0.7
	elseif CarCalculator.CarInfo.TrimInfo.Tyres.Name == "TyreType_SportComp_Name" then
		Data.TreadCoeff = 0.2
		Data.SoftnessCoef = 0.6
		Data.TireStiffnessCoef = 0.9
		Data.LoadSenseCoef = 0.87
	elseif CarCalculator.CarInfo.TrimInfo.Tyres.Name == "TyreType_SemiSlick_Name" then
		Data.TreadCoeff = 0
		Data.SoftnessCoef = 1.0
		Data.TireStiffnessCoef = 1
		Data.LoadSenseCoef = 1.05
	end


	local function  GenerateTyreData(Data, WheelPosition, WheelParameters)
		local TyreParams = WheelParameters

		--preliminary calculations--
		--=================================================--

		local ScaleMultiplier = (Data.WheelBase / 230.0) * 0.5 + (TyreDetails.Front.OverallDiameter * 0.1 / 65.0) * 0.5
		Data.ScaleMultiplier = ScaleMultiplier
		local radius = TyreDetails.Front.OverallDiameter * 0.001 * 0.5
		local hubRadius = TyreDetails.Front.Rim * 2.54 * 0.5 * 0.01 + 0.0254
		local numRays = 16
		local wheelOffset = 0
		local hubWidth = TyreDetails[WheelPosition].Width * 0.001
		local tireWidth = TyreDetails[WheelPosition].Width * 0.001
		local axleWidth = 0.2 * ScaleMultiplier

		local suspensionBaseWeight = SuspensionTypeWeights[CarInfo.PlatformInfo[WheelPosition .. "Suspension"].Name][WheelPosition]

		local tirePeripheryLength  = 2 * math.pi * radius / numRays
		local hubPeripheryLength = 2 * math.pi * hubRadius / numRays

		local frontSpacing = -(axleWidth * 0.5 - hubWidth * 0.5 + wheelOffset)
		local backSpacing = -(axleWidth * 0.5 - hubWidth * 0.5 - wheelOffset)



	end -- function GenerateTyreData

	GenerateTyreData(Data, "Front", FrontWheelParameters)
	GenerateTyreData(Data, "Rear", RearWheelParameters)

	return Data
end

function CExporter.ExportCarFiles(CarCalculator)
	local CarInfo = CarCalculator.CarInfo

	local FileContents = "This contains string data for the car"

	return  { ["ExportedCar.txt"] = FileContents }
end