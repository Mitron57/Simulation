#ifndef LIFESYSTEM_H
#define LIFESYSTEM_H
namespace Solution {
    struct LifeSystem final : System {
        LifeSystem() = default;

        void onAwake() override;
        bool onUpdate() override;

    private:
        static bool calculateChance(std::double_t chance, std::double_t coeff = 1);
        template <typename... T>
        static void generateEntity(std::uint32_t count);
        static void generateGrass();

        template <typename T>
        static void bornChild(std::int32_t posX, std::int32_t posY);

        template <typename T>
        static std::tuple<bool, std::int32_t, std::int32_t> isNear(
            const std::shared_ptr<Position>& position
        );

        template <typename T, typename F>
        void life(std::shared_ptr<Entity>& entity);
    };
}  // namespace Solution
#endif  // LIFESYSTEM_H
