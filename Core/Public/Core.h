// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Misc/MonolithicHeaderBoilerplate.h"
MONOLITHIC_HEADER_BOILERPLATE()

/*----------------------------------------------------------------------------
	Low level includes.
----------------------------------------------------------------------------*/

#include "CoreMinimal.h"
#include "Containers/ContainersFwd.h"
#include "GenericPlatform/GenericPlatformAffinity.h"
#include "GenericPlatform/GenericPlatformAtomics.h"
#include "GenericPlatform/GenericPlatformCriticalSection.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "GenericPlatform/GenericPlatformMemory.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "GenericPlatform/GenericPlatformNamedPipe.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "GenericPlatform/GenericPlatformProperties.h"
#include "GenericPlatform/GenericPlatformStackWalk.h"
#include "GenericPlatform/GenericPlatformStricmp.h"
#include "GenericPlatform/GenericPlatformString.h"
#include "GenericPlatform/GenericPlatformTime.h"
#include "GenericPlatform/GenericPlatformTLS.h"
#include "HAL/CriticalSection.h"
#include "HAL/PlatformAffinity.h"
#include "HAL/PlatformAtomics.h"
#include "HAL/PlatformCrt.h"
#include "HAL/PlatformFile.h"
#include "HAL/PlatformFileManager.h"
#include "HAL/PlatformIncludes.h"
#include "HAL/PlatformMath.h"
#include "HAL/PlatformMemory.h"
#include "HAL/PlatformMisc.h"
#include "HAL/PlatformNamedPipe.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformProperties.h"
#include "HAL/PlatformStackWalk.h"
#include "HAL/PlatformString.h"
#include "HAL/PlatformTime.h"
#include "HAL/PlatformTLS.h"
#include "HAL/UnrealMemory.h"
#include "Internationalization/TextNamespaceFwd.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"
#include "Math/NumericLimits.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/AssertionMacros.h"
#include "Misc/Char.h"
#include "Misc/CString.h"
#include "Misc/DateTime.h"
#include "Misc/EngineVersionBase.h"
#include "Misc/Exec.h"
#include "Misc/MessageDialog.h"
#include "Misc/OutputDevice.h"
#include "Misc/Timespan.h"
#include "Misc/VarArgs.h"
#include "Templates/AlignmentTemplates.h"
#include "Templates/AndOrNot.h"
#include "Templates/ChooseClass.h"
#include "Templates/Decay.h"
#include "Templates/EnableIf.h"
#include "Templates/Function.h"
#include "Templates/Greater.h"
#include "Templates/IntegralConstant.h"
#include "Templates/Invoke.h"
#include "Templates/IsArithmetic.h"
#include "Templates/IsClass.h"
#include "Templates/IsEnumClass.h"
#include "Templates/IsFloatingPoint.h"
#include "Templates/IsIntegral.h"
#include "Templates/IsPODType.h"
#include "Templates/IsPointer.h"
#include "Templates/IsSigned.h"
#include "Templates/IsTriviallyCopyAssignable.h"
#include "Templates/IsTriviallyCopyConstructible.h"
#include "Templates/IsTriviallyDestructible.h"
#include "Templates/IsUECoreType.h"
#include "Templates/Less.h"
#include "Templates/MemoryOps.h"
#include "Templates/PointerIsConvertibleFromTo.h"
#include "Templates/RemoveReference.h"
#include "Templates/TypeCompatibleBytes.h"
#include "Templates/UnrealTemplate.h"
#include "Templates/UnrealTypeTraits.h"
#include "Traits/IsContiguousContainer.h"
#include "UObject/UnrealNames.h"

#include "Misc/CoreDefines.h"

// Container forward declarations
#include "Containers/ContainerAllocationPolicies.h"

#include "UObject/UObjectHierarchyFwd.h"

#include "CoreGlobals.h"

/*----------------------------------------------------------------------------
	Includes.
----------------------------------------------------------------------------*/

#include "Algo/Reverse.h"
#include "Async/AsyncWork.h"
#include "Containers/Array.h"
#include "Containers/ArrayBuilder.h"
#include "Containers/ArrayView.h"
#include "Containers/BitArray.h"
#include "Containers/ChunkedArray.h"
#include "Containers/CircularBuffer.h"
#include "Containers/CircularQueue.h"
#include "Containers/DynamicRHIResourceArray.h"
#include "Containers/EnumAsByte.h"
#include "Containers/IndirectArray.h"
#include "Containers/List.h"
#include "Containers/LockFreeFixedSizeAllocator.h"
#include "Containers/LockFreeList.h"
#include "Containers/Map.h"
#include "Containers/MapBuilder.h"
#include "Containers/MRUArray.h"
#include "Containers/Queue.h"
#include "Containers/ResourceArray.h"
#include "Containers/ScriptArray.h"
#include "Containers/Set.h"
#include "Containers/SparseArray.h"
#include "Containers/StaticArray.h"
#include "Containers/StaticBitArray.h"
#include "Containers/StringConv.h"
#include "Containers/Ticker.h"
#include "Containers/UnrealString.h"
#include "Delegates/Delegate.h"
#include "Delegates/DelegateBase.h"
#include "Delegates/DelegateSettings.h"
#include "Delegates/IDelegateInstance.h"
#include "Delegates/IntegerSequence.h"
#include "Delegates/MulticastDelegateBase.h"
#include "Features/IModularFeature.h"
#include "HAL/Event.h"
#include "HAL/FileManager.h"
#include "HAL/IConsoleManager.h"
#include "HAL/MemoryBase.h"
#include "HAL/OutputDevices.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "HAL/ThreadingBase.h"
#include "HAL/ThreadManager.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/ThreadSafeCounter.h"
#include "HAL/ThreadSafeCounter64.h"
#include "HAL/ThreadSingleton.h"
#include "HAL/TlsAutoCleanup.h"
#include "Internationalization/Culture.h"
#include "Internationalization/CulturePointer.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/ITextData.h"
#include "Internationalization/Text.h"
#include "Internationalization/TextLocalizationManager.h"
#include "Internationalization/TextLocalizationManagerGlobals.h"
#include "Logging/LogScopedCategoryAndVerbosityOverride.h"
#include "Logging/LogScopedVerbosityOverride.h"
#include "Logging/LogSuppressionInterface.h"
#include "Math/Axis.h"
#include "Math/Box.h"
#include "Math/Box2D.h"
#include "Math/BoxSphereBounds.h"
#include "Math/CapsuleShape.h"
#include "Math/ClipProjectionMatrix.h"
#include "Math/Color.h"
#include "Math/ColorList.h"
#include "Math/ConvexHull2d.h"
#include "Math/CurveEdInterface.h"
#include "Math/Edge.h"
#include "Math/Float16.h"
#include "Math/Float16Color.h"
#include "Math/Float32.h"
#include "Math/InterpCurve.h"
#include "Math/InterpCurvePoint.h"
#include "Math/Interval.h"
#include "Math/IntPoint.h"
#include "Math/IntRect.h"
#include "Math/IntVector.h"
#include "Math/InverseRotationMatrix.h"
#include "Math/Matrix.h"
#include "Math/MirrorMatrix.h"
#include "Math/OrientedBox.h"
#include "Math/OrthoMatrix.h"
#include "Math/PerspectiveMatrix.h"
#include "Math/Plane.h"
#include "Math/Quat.h"
#include "Math/QuatRotationTranslationMatrix.h"
#include "Math/RandomStream.h"
#include "Math/Range.h"
#include "Math/RangeBound.h"
#include "Math/RangeSet.h"
#include "Math/RotationAboutPointMatrix.h"
#include "Math/RotationMatrix.h"
#include "Math/RotationTranslationMatrix.h"
#include "Math/Rotator.h"
#include "Math/ScaleMatrix.h"
#include "Math/ScaleRotationTranslationMatrix.h"
#include "Math/SHMath.h"
#include "Math/Sphere.h"
#include "Math/Transform.h"
#include "Math/TranslationMatrix.h"
#include "Math/TwoVectors.h"
#include "Math/UnrealMath.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "Math/Vector2DHalf.h"
#include "Math/Vector4.h"
#include "Math/VectorRegister.h"
#include "Misc/Application.h"
#include "Misc/Attribute.h"
#include "Misc/AutomationTest.h"
#include "Misc/BufferedOutputDevice.h"
#include "Misc/ByteSwap.h"
#include "Misc/CallbackDevice.h"
#include "Misc/CommandLine.h"
#include "Misc/Compression.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/CoreDelegates.h"
#include "Misc/CoreMisc.h"
#include "Misc/CoreStats.h"
#include "Misc/Crc.h"
#include "Misc/EnumClassFlags.h"
#include "Misc/EnumRange.h"
#include "Misc/FeedbackContext.h"
#include "Misc/FileHelper.h"
#include "Misc/Guid.h"
#include "Misc/IQueuedWork.h"
#include "Misc/ITransaction.h"
#include "Misc/LocalTimestampDirectoryVisitor.h"
#include "Misc/MemStack.h"
#include "Misc/MonitoredProcess.h"
#include "Misc/NetworkGuid.h"
#include "Misc/NoopCounter.h"
#include "Misc/ObjectThumbnail.h"
#include "Misc/Optional.h"
#include "Misc/OutputDeviceAnsiError.h"
#include "Misc/OutputDeviceArchiveWrapper.h"
#include "Misc/OutputDeviceConsole.h"
#include "Misc/OutputDeviceDebug.h"
#include "Misc/OutputDeviceError.h"
#include "Misc/OutputDeviceFile.h"
#include "Misc/OutputDeviceHelper.h"
#include "Misc/OutputDeviceMemory.h"
#include "Misc/OutputDeviceNull.h"
#include "Misc/OutputDeviceRedirector.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "Misc/QueuedThreadPool.h"
#include "Misc/ScopedEvent.h"
#include "Misc/ScopedSlowTask.h"
#include "Misc/ScopeLock.h"
#include "Misc/SingleThreadEvent.h"
#include "Misc/SingleThreadRunnable.h"
#include "Misc/SlowTask.h"
#include "Misc/SlowTaskStack.h"
#include "Misc/StringUtility.h"
#include "Misc/StructBuilder.h"
#include "Misc/TimeGuard.h"
#include "Misc/Variant.h"
#include "Misc/WildcardString.h"
#include "ProfilingDebugging/ExternalProfiler.h"
#include "ProfilingDebugging/ProfilingHelpers.h"
#include "ProfilingDebugging/ScopedDebugInfo.h"
#include "Serialization/Archive.h"
#include "Serialization/ArchiveProxy.h"
#include "Serialization/ArrayReader.h"
#include "Serialization/ArrayWriter.h"
#include "Serialization/BitReader.h"
#include "Serialization/BitWriter.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/BufferReader.h"
#include "Serialization/BufferWriter.h"
#include "Serialization/CustomVersion.h"
#include "Serialization/LargeMemoryReader.h"
#include "Serialization/LargeMemoryWriter.h"
#include "Serialization/MemoryArchive.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/NameAsStringProxyArchive.h"
#include "Stats/Stats.h"
#include "Stats/StatsMisc.h"
#include "Templates/IsArray.h"
#include "Templates/RefCounting.h"
#include "Templates/RemoveExtent.h"
#include "Templates/ScopedCallback.h"
#include "Templates/SharedPointer.h"
#include "Templates/Sorting.h"
#include "Templates/Tuple.h"
#include "Templates/TypeHash.h"
#include "Templates/UniqueObj.h"
#include "Templates/UniquePtr.h"
#include "UObject/BlueprintsObjectVersion.h"
#include "UObject/BuildObjectVersion.h"
#include "UObject/CoreObjectVersion.h"
#include "UObject/EditorObjectVersion.h"
#include "UObject/FrameworkObjectVersion.h"
#include "UObject/MobileObjectVersion.h"
#include "UObject/NameTypes.h"
#include "UObject/NetworkingObjectVersion.h"
#include "UObject/ObjectVersion.h"
#include "UObject/OnlineObjectVersion.h"
#include "UObject/PhysicsObjectVersion.h"
#include "UObject/PlatformObjectVersion.h"
#include "UObject/RenderingObjectVersion.h"
#include "UObject/ScriptDelegates.h"
#include "UObject/SequencerObjectVersion.h"
#include "UObject/VRObjectVersion.h"
#include "UObject/WeakObjectPtrTemplates.h"
